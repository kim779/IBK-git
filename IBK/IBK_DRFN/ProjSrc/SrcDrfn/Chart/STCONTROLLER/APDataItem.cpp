// APDataItem.cpp: implementation of the CAPDataItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APDataItem.h"
#include "ApQurier.h"
#include "STController.h"
#include "../Common_ST/STConfigDef.h"
#include "../Include_ST/Commonlib.h"

//#include "../include/DRDSDEF.H"
//#include "../../COMMON_BASE/drds/DRDSDEF.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define QUOTESYMBOLSIZE			6 //�ֽ� �ɼ�
#define FUOPTSYMBOLSIZE			8 //���� �ɼ�

//@@@@@06.11.21 FUOPTSYMBOLSIZE_SS �߰�, �ڸ����� 5�ΰ�춧����
#define FUOPTSYMBOLSIZE_SS		5 //���� �ڸ����� 5�ڸ��� �� ���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAPDataItem::CAPDataItem()
{
	m_pApQurier = NULL;
	m_chType = '2'; // ƽ'0'/��'1'/��'2'/��'3'/��'4'
	m_lTickMinNumber = 1;
	m_lCntOfDrdsForTickNumber = 0;
	m_lTimeNext = 0;
//	m_dDayOpen = 0;
//	m_dDayHigh = 0;
//	m_dDayLow = 0;
//	m_dDayClose = 0;

	m_bIsExternaRawData = FALSE;
	m_bIsMultiBong = FALSE;
	m_pSdataItemDate = new CSDataItem;
	m_pDataItemOpen = new CSTDataItem;
	m_pDataItemHigh = new CSTDataItem;
	m_pDataItemLow = new CSTDataItem;
	m_pDataItemClose = new CSTDataItem;
	m_pDataItemVolume = new CSTDataItem;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	m_pDataItemExt = new CSTDataItem;
	//KHD : 2006. 12.12
	m_dPrevOpenData	= 0;
	m_dPrevCloseData	= 0;
	m_dPrevLowData	= 0;	
	m_dPrevHighData	= 0;
	//

	RemoveAllData();
}

CAPDataItem::~CAPDataItem()
{
	RemoveAllData();
	if(!m_bIsExternaRawData)
	{
		delete m_pSdataItemDate;
		delete m_pDataItemOpen;
		delete m_pDataItemHigh;
		delete m_pDataItemLow;
		delete m_pDataItemClose;
		delete m_pDataItemVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		delete m_pDataItemExt;
	}
	else if(m_bIsMultiBong)
	{
		delete m_pSdataItemDate;
		delete m_pDataItemOpen;
		delete m_pDataItemHigh;
		delete m_pDataItemLow;
		delete m_pDataItemClose;
	}

	m_pSdataItemDate =NULL;
	m_pDataItemOpen =NULL;
	m_pDataItemHigh =NULL;
	m_pDataItemLow =NULL;
	m_pDataItemClose =NULL;
	m_pDataItemVolume =NULL;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	m_pDataItemExt =NULL;
}

// Returned Value means whether AP query is needed or not.
BOOL CAPDataItem::RequestDataToServer(CCommentData* pBasicalInputData,CSTController *pSTController,CApQurier* pApQurier)
{
	
	if(pBasicalInputData==NULL) 
	{
		::AfxMessageBox("pBasicalInputData �� �������� �ʽ��ϴ�.");
		return FALSE;
	}
	RemoveAllData();

	ST_SEND2SERVER st;
	m_pApQurier = pApQurier;
	int nShifter = 0;
	int nPos = 0;
	CString strDataDivided;
	CString strTitle;
	CString strContents;
	CString strTemp;//KHD : 2006.12.12

	////////////////////////////////////////////////////////
	// Server AP is required...
	CString strSymbol;			// SYMBOL
	CString strEndDate;			// ENDDATE
	CString strAllCount;		// ALLCOUNT
	CString strType;			// TYPE
	CString strTickMinNumber;	// TMNUMBER
	///////////////////////////////////////////////////////
	
	CSDataItem	*pSDataItemDate = NULL;
	CSTDataItem	*pDataItemOpen = NULL;
	CSTDataItem	*pDataItemHigh = NULL;
	CSTDataItem	*pDataItemLow = NULL;
	CSTDataItem	*pDataItemClose = NULL;
	CSTDataItem	*pDataItemVolume = NULL;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	CSTDataItem	*pDataItemExt = NULL;
	///////////////////////////////////////////////////////

	pSDataItemDate = (CSDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_DATEPOINTER));
	pDataItemOpen = (CSTDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_OPENPOINTER));
	pDataItemHigh = (CSTDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_HIGHPOINTER));
	pDataItemLow = (CSTDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_LOWPOINTER));
	pDataItemClose = (CSTDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_CLOSEPOINTER));
	pDataItemVolume = (CSTDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_VOLUMEPOINTER));
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	pDataItemExt = (CSTDataItem*)atol(pBasicalInputData->GetComment(EXECUTE_EXTDATA));

	strSymbol = pBasicalInputData->GetComment(EXECUTE_SYMBOL);
	strEndDate = pBasicalInputData->GetComment(EXECUTE_ENDDATE);
	strAllCount = pBasicalInputData->GetComment(EXECUTE_ALLCOUNT);
	strType = pBasicalInputData->GetComment(EXECUTE_TYPE);
	strTickMinNumber = pBasicalInputData->GetComment(EXECUTE_TMNUMBER);
	m_strTimeFormat = pBasicalInputData->GetComment(EXECUTE_TIMETYPE);
	//�����ؼ� ������ �־�� �Ѵ�. KHD: 2006.12.12
				
	SetHelpMessageData(pBasicalInputData->GetComment(EXECUTE_PREVDATA));
	//
	if(m_strTimeFormat == "YYYYMMDD")		// "YYYYMMDD" is regarded as TIME_DAY
	{
		m_chType=TIME_DAY;
	}
	else if(m_strTimeFormat == "MMDDHHMM") // "MMDDHHMM" is regarded as TIME_MIN
	{
		m_chType=TIME_MIN;
	}
	else if(m_strTimeFormat == "DDHHMMSS")
	{
		m_chType=TIME_TICK;
	}

	if(strSymbol.GetLength()==QUOTESYMBOLSIZE)	// �ֽ� AP��ȸ...
	{
		TR10610_I stOutput;	
		memset(&stOutput,0x20,sizeof(stOutput)); stOutput.chNull = NULL;
		strTemp.Format("%d",(long)this);
		memcpy(stOutput.sBuffer,(LPCTSTR)strTemp,strTemp.GetLength());

		memcpy(stOutput.shcode,(LPCTSTR)strSymbol,strSymbol.GetLength());
		m_strCode = strSymbol;
		memcpy(stOutput.date,(LPCTSTR)strEndDate,strEndDate.GetLength());
		strTemp.Format("%04d",atol(strAllCount));
		memcpy(stOutput.num,(LPCTSTR)strTemp,4);
		
		m_chType = strType[0];
		stOutput.button[0] = strType[0];

		strTemp.Format("%2s",strTickMinNumber);
		memcpy(stOutput.tick,(LPCTSTR)strTemp,sizeof(stOutput.tick));
		m_lTickMinNumber = atol(strTemp);		
		stOutput.nflag[0] = '0';
		stOutput.ImgRgb[0] = '1';          // '0':����� '1':�Ǻ�              

		st.m_pBuffer = (char*)&stOutput;
		st.m_lLength= sizeof(stOutput) - 1;
		st.m_strTRCode = "HWS10610";
		st.m_pRequester = this;
		//KHD : ���⼭ �����͸� �޾ƾ� �Ѵ�. :2006.08.03
		::SendMessage(m_pApQurier->GetSafeHwnd(),UMSG_ST_SEND2SERVER,(WPARAM)&st,NULL);
	}
	//@@@@@06.11.21 FUOPTSYMBOLSIZE_SS �߰�, �ڸ����� 5�ΰ�춧����
	else if(strSymbol.GetLength()==FUOPTSYMBOLSIZE || strSymbol.GetLength()==FUOPTSYMBOLSIZE_SS)	// ����/�ɼ� AP��ȸ
	{
		TR10611_I stOutput;	
		memset(&stOutput,0x20,sizeof(stOutput)); stOutput.chNull = NULL;
		strTemp.Format("%d",(long)this);
		memcpy(stOutput.sBuffer,(LPCTSTR)strTemp,strTemp.GetLength());
		
		memcpy(stOutput.sCode,(LPCTSTR)strSymbol,strSymbol.GetLength());
		m_strCode = strSymbol;
		memcpy(stOutput.sDate,(LPCTSTR)strEndDate,strEndDate.GetLength());

		strTemp.Format("%04d",atol(strAllCount));
		memcpy(stOutput.sFetchCnt,(LPCTSTR)strTemp,4);
		
		m_chType = strType[0];
		stOutput.sBtnGb[0] = strType[0];

		strTemp.Format("%2s",strTickMinNumber);
		memcpy(stOutput.sTick,(LPCTSTR)strTemp,sizeof(stOutput.sTick));
		m_lTickMinNumber = atol(strTemp);
		
		stOutput.nFlag[0] = '0';

		st.m_pBuffer = (char*)&stOutput;
		st.m_lLength= sizeof(stOutput) - 1;
		st.m_strTRCode = "HWS10611";
		st.m_pRequester = this;
		::SendMessage(m_pApQurier->GetSafeHwnd(),UMSG_ST_SEND2SERVER,(WPARAM)&st,NULL);
	}
	else if(strSymbol.GetLength() == 0)// �ܺ��� �ð����� �����͸� �����Ѵ�....
	{
		if(pSDataItemDate&&pDataItemOpen&&pDataItemHigh&&pDataItemLow&&pDataItemClose&&pDataItemVolume)
		{
			delete m_pSdataItemDate;
			delete m_pDataItemOpen;
			delete m_pDataItemHigh;
			delete m_pDataItemLow;
			delete m_pDataItemClose;
			delete m_pDataItemVolume;
			m_bIsExternaRawData = TRUE;
			m_pSdataItemDate = pSDataItemDate;
			m_pDataItemOpen = pDataItemOpen;
			m_pDataItemHigh = pDataItemHigh;
			m_pDataItemLow = pDataItemLow;
			m_pDataItemClose = pDataItemClose;
			m_pDataItemVolume = pDataItemVolume;

			//TRACE("CAPDataItem::RequestDataToServer => Log Start...\n");
			//TRACE("dwDataTime=%ld, dwDataOpen=%ld\n", pSDataItemDate, pDataItemOpen);
			//TRACE("CAPDataItem::RequestDataToServer => Log End...\n");

			// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
			if(pDataItemExt)
			{
				delete m_pDataItemExt;
				m_pDataItemExt = pDataItemExt;
			}

			return FALSE;
		}		
	}
	else
	{
		TR10610_I stOutput;	
		memset(&stOutput,0x20,sizeof(stOutput)); stOutput.chNull = NULL;
		strTemp.Format("%d",(long)this);
		memcpy(stOutput.sBuffer,(LPCTSTR)strTemp,strTemp.GetLength());

		memcpy(stOutput.shcode,(LPCTSTR)strSymbol,strSymbol.GetLength());
		m_strCode = strSymbol;
		memcpy(stOutput.date,(LPCTSTR)strEndDate,strEndDate.GetLength());
		strTemp.Format("%04d",atol(strAllCount));
		memcpy(stOutput.num,(LPCTSTR)strTemp,4);
		
		m_chType = strType[0];
		stOutput.button[0] = strType[0];

		strTemp.Format("%2s",strTickMinNumber);
		memcpy(stOutput.tick,(LPCTSTR)strTemp,sizeof(stOutput.tick));
		m_lTickMinNumber = atol(strTemp);		
		stOutput.nflag[0] = '0';
		stOutput.ImgRgb[0] = '1';          // '0':����� '1':�Ǻ�              

		st.m_pBuffer = (char*)&stOutput;
		st.m_lLength= sizeof(stOutput) - 1;
		st.m_strTRCode = "HWS10610";
		st.m_pRequester = this;
		//KHD : ���⼭ �����͸� �޾ƾ� �Ѵ�. :2006.08.03
		::SendMessage(m_pApQurier->GetSafeHwnd(),UMSG_ST_SEND2SERVER,(WPARAM)&st,NULL);
	}
	
	m_lCntOfDrdsForTickNumber = 0;
	return TRUE;
}

//Add by : KHD : 2007.01.02
void CAPDataItem::SetHelpMessageData(CString stMessage)
{
	if(stMessage.GetLength() && stMessage.Find(':')>0)
	{
		m_dPrevOpenData = atof(stMessage.Left(stMessage.Find(':')));
		stMessage.Delete(0,stMessage.Find(':')+1);

		m_dPrevHighData = atof(stMessage.Left(stMessage.Find(':')));
		stMessage.Delete(0,stMessage.Find(':')+1);

		m_dPrevLowData  = atof(stMessage.Left(stMessage.Find(':')));
		stMessage.Delete(0,stMessage.Find(':')+1);				

		m_dPrevCloseData   = atof(stMessage);
	}
}

void CAPDataItem::SetDataItemSize(int nSize)
{
	m_pSdataItemDate->SetSize(nSize);
	m_pDataItemOpen->SetSize(nSize);
	m_pDataItemHigh->SetSize(nSize);
	m_pDataItemLow->SetSize(nSize);
	m_pDataItemClose->SetSize(nSize);
	m_pDataItemVolume->SetSize(nSize);
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	m_pDataItemExt->SetSize(nSize);
}

// �ý��� Ʈ���̵� ȯ�漳���� ���� �ӽ��� ���ۼ����Ѵ�.
BOOL CAPDataItem::SetTempData()
{
	SetDataItemSize(1);
	return TRUE;
}


BOOL CAPDataItem::ReceiveAPData(ST_RECEIVEFROMSERVER* pStR)
{
	CString strDate;
	int nCntAll = 0;
	if(m_strCode.GetLength()==QUOTESYMBOLSIZE)
	{
		if(pStR->m_dwLength>sizeof(TR10610_O))
		{			
			AfxMessageBox("Buffer Overflow!!");
			return FALSE;
		}
		TR10610_O* pstOutput = (TR10610_O*)pStR->m_pBuffer;	
		char szTemp[18];
		MEMCPYWITHNULL(szTemp,pstOutput->tmp);
		nCntAll = atoi(szTemp);
		SetDataItemSize(nCntAll);
		m_pSdataItemDate->SetType(m_chType);
		if(m_chType==TIME_DAY)
		{
			m_strTimeFormat = "YYYYMMDD";
		}
		else if(m_chType==TIME_MIN)
		{
			m_strTimeFormat = "MMDDHHMM";
		}
		else if(m_chType==TIME_TICK)
		{
			m_strTimeFormat = "DDHHMMSS";
		}

		for(int nIndex=0;nIndex<nCntAll;nIndex++)
		{
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].date);
			m_pSdataItemDate->SetAt(nIndex,szTemp);
			strDate = szTemp;
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].open);
			m_pDataItemOpen->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].high);
			m_pDataItemHigh->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].low);
			m_pDataItemLow->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].close);
			m_pDataItemClose->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].volume);
			m_pDataItemVolume->SetAt(nIndex,atof(szTemp));
		}
	}
	//@@@@@06.11.21 FUOPTSYMBOLSIZE_SS �߰�, �ڸ����� 5�ΰ�춧����
	else if(m_strCode.GetLength()==FUOPTSYMBOLSIZE || m_strCode.GetLength()==FUOPTSYMBOLSIZE_SS)
	{
		if(pStR->m_dwLength>sizeof(TR10611_O))
		{			
			AfxMessageBox("Buffer Overflow!!");
			return FALSE;
		}
		TR10611_O* pstOutput = (TR10611_O*)pStR->m_pBuffer;	
		char szTemp[18];
		MEMCPYWITHNULL(szTemp,pstOutput->Cchtcnt);
		nCntAll = atoi(szTemp);
		SetDataItemSize(nCntAll);
		m_pSdataItemDate->SetType(m_chType);
		if(m_chType==TIME_DAY)
		{
			m_strTimeFormat = "YYYYMMDD";
		}
		else if(m_chType==TIME_MIN)
		{
			m_strTimeFormat = "MMDDHHMM";
		}
		else if(m_chType==TIME_TICK)
		{
			m_strTimeFormat = "DDHHMMSS";
		}
		
		for(int nIndex=0;nIndex<nCntAll;nIndex++)
		{
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].date);
			m_pSdataItemDate->SetAt(nIndex,szTemp);
			strDate = szTemp;
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].open);
			m_pDataItemOpen->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].high);
			m_pDataItemHigh->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].low);
			m_pDataItemLow->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].close);
			m_pDataItemClose->SetAt(nIndex,atof(szTemp));
			MEMCPYWITHNULL(szTemp,pstOutput->chart[nIndex].volume);
			m_pDataItemVolume->SetAt(nIndex,atof(szTemp));
		}
	}
	if(nCntAll&&m_chType==TIME_MIN)// '��' ����Ÿ�� ��� �к������� ����� ���� �к��� �ֱ⸦ �����Ѵ�.
	{
		SetTimeNext(strDate);
	}	
//	m_pApQurier->OnCodeAdvise(this);// �ǽð� ������ ���� �����Ѵ�.

	// �� �����͸� �����ϴ� CSTController���� AP�� �޾Ҵٴ� ��ȣ�� ������.
	CSTController* pSTController = NULL;
	POSITION pos = m_listSTController.GetHeadPosition();
	while(pos)
	{
		pSTController = m_listSTController.GetNext(pos);
		pSTController->OnReceiveQuoteData();
	}
	return TRUE;
}

//KHD : 2006.08.03 : íƮ �����͸� ���⼭ �޾ƺ���!!
BOOL CAPDataItem::ReceiveAPData()
{
	CString strDate;
	int nCntAll = 0;
	//KHD : �������̽�... �ε�..

	ISTAnalysisInput* pMng ;
	ISTInterface* pSTInterface = (ISTInterface*)m_pChartItem;
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	LONG dwDataTime=0, dwDataOpen=0, dwDataHigh=0, dwDataLow=0, dwDataClose=0,dwDataVolume=0, dwDataExt = 0;
	BOOL bIsMultiBong = FALSE;
	if(pSTInterface)
	{
		pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
		int nCnt = 0;
		if(pMng->IsUseUserData()) 
		{
			ISTUserData* pUserData = pMng->GetUserDataInterface();
			int nBong = 1;
			// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
			nCnt = pUserData->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose,dwDataVolume, dwDataExt, nBong);

			if(nBong > 1)
				bIsMultiBong = TRUE;
		}
		else
		{
			// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
			nCnt = pMng->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose,dwDataVolume, dwDataExt);
		}

		CString str;
		str.Format("%d",pMng->GetDivideState());
		m_chType = str[0];
		//memcpy(m_chType,str,sizeof(m_chType));
	}
	if(m_strCode.GetLength()==QUOTESYMBOLSIZE)//�ֽ�
	{
		delete m_pSdataItemDate;
		delete m_pDataItemOpen;
		delete m_pDataItemHigh;
		delete m_pDataItemLow;
		delete m_pDataItemClose;
		delete m_pDataItemVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		delete m_pDataItemExt;
		
		m_bIsExternaRawData = TRUE;
		m_bIsMultiBong = bIsMultiBong;

		m_pSdataItemDate = (CSDataItem*)dwDataTime;
		m_pDataItemOpen = (CSTDataItem*)dwDataOpen;
		m_pDataItemHigh =  (CSTDataItem*)dwDataHigh;
		m_pDataItemLow = (CSTDataItem*)dwDataLow;
		m_pDataItemClose = (CSTDataItem*)dwDataClose;
		m_pDataItemVolume = (CSTDataItem*)dwDataVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		m_pDataItemExt = (CSTDataItem*)dwDataExt;
		
		m_pSdataItemDate->SetType(m_chType);
		// m_pSdataItemDate->SetType('2');
		// char chType = m_pSdataItemDate->GetType();
		if(m_chType==TIME_DAY)
		{
			m_strTimeFormat = "YYYYMMDD";
		}
		else if(m_chType==TIME_MIN)
		{
			m_strTimeFormat = "MMDDHHMM";
		}
		else if(m_chType==TIME_TICK)
		{
			m_strTimeFormat = "DDHHMMSS";
		}

		//for(int i = 0 ; i< m_pDataItemOpen->GetSize();i++)
		//{
		//	TRACE("DATA =%f ", m_pDataItemOpen->GetAt(i));
		//}
		//m_pSdataItemDate = (CSDataItem*)dwDataTime;
		//m_pDataItemOpen = (CSTDataItem*)dwDataOpen;
		//m_pDataItemHigh = (CSTDataItem*)dwDataHigh;
		//m_pDataItemLow= (CSTDataItem*)dwDataLow;
		//m_pDataItemClose = (CSTDataItem*)dwDataClose;
		//m_pDataItemVolume = dwDataVolume;		
	}
	//@@@@@06.11.21 FUOPTSYMBOLSIZE_SS �߰�, �ڸ����� 5�ΰ�춧����
	else if(m_strCode.GetLength()==FUOPTSYMBOLSIZE||m_strCode.GetLength()==FUOPTSYMBOLSIZE_SS)//����Ÿ��
	{
		delete m_pSdataItemDate;
		delete m_pDataItemOpen;
		delete m_pDataItemHigh;
		delete m_pDataItemLow;
		delete m_pDataItemClose;
		delete m_pDataItemVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		delete m_pDataItemExt;

		m_bIsExternaRawData = TRUE;
		m_bIsMultiBong = bIsMultiBong;

		m_pSdataItemDate = (CSDataItem*)dwDataTime;
		m_pDataItemOpen = (CSTDataItem*)dwDataOpen;
		m_pDataItemHigh =  (CSTDataItem*)dwDataHigh;
		m_pDataItemLow = (CSTDataItem*)dwDataLow;
		m_pDataItemClose = (CSTDataItem*)dwDataClose;
		m_pDataItemVolume = (CSTDataItem*)dwDataVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		m_pDataItemExt = (CSTDataItem*)dwDataExt;

		m_pSdataItemDate->SetType(m_chType);
		// m_pSdataItemDate->SetType('2');
		// char chType = m_pSdataItemDate->GetType();
		if(m_chType==TIME_DAY)
		{
			m_strTimeFormat = "YYYYMMDD";
		}
		else if(m_chType==TIME_MIN)
		{
			m_strTimeFormat = "MMDDHHMM";
		}
		else if(m_chType==TIME_TICK)
		{
			m_strTimeFormat = "DDHHMMSS";
		}

		for(int i = 0 ; i< m_pDataItemOpen->GetSize();i++)
		{
			TRACE("DATA =%f ", m_pDataItemOpen->GetAt(i));
		}
	}
	else if(m_strCode.GetLength()>0)//�ֽ�
	{
		delete m_pSdataItemDate;
		delete m_pDataItemOpen;
		delete m_pDataItemHigh;
		delete m_pDataItemLow;
		delete m_pDataItemClose;
		delete m_pDataItemVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		delete m_pDataItemExt;
		
		m_bIsExternaRawData = TRUE;
		m_bIsMultiBong = bIsMultiBong;

		m_pSdataItemDate = (CSDataItem*)dwDataTime;
		m_pDataItemOpen = (CSTDataItem*)dwDataOpen;
		m_pDataItemHigh =  (CSTDataItem*)dwDataHigh;
		m_pDataItemLow = (CSTDataItem*)dwDataLow;
		m_pDataItemClose = (CSTDataItem*)dwDataClose;
		m_pDataItemVolume = (CSTDataItem*)dwDataVolume;
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		m_pDataItemExt = (CSTDataItem*)dwDataExt;
		
		m_pSdataItemDate->SetType(m_chType);
		// m_pSdataItemDate->SetType('2');
		// char chType = m_pSdataItemDate->GetType();
		if(m_chType==TIME_DAY)
		{
			m_strTimeFormat = "YYYYMMDD";
		}
		else if(m_chType==TIME_MIN)
		{
			m_strTimeFormat = "MMDDHHMM";
		}
		else if(m_chType==TIME_TICK)
		{
			m_strTimeFormat = "DDHHMMSS";
		}
	}

	// �� �����͸� �����ϴ� CSTController���� AP�� �޾Ҵٴ� ��ȣ�� ������.
	CSTController* pSTController = NULL;
	POSITION pos = m_listSTController.GetHeadPosition();
	while(pos)
	{
		pSTController = m_listSTController.GetNext(pos);
		pSTController->OnReceiveQuoteData();
	}
	return TRUE;
}

void CAPDataItem::SetTimeNext(LPCTSTR lpDateLast)
{
	m_lTimeNext = atol(lpDateLast) + m_lTickMinNumber;
	if(m_lTimeNext%100>=60)
	{
		m_lTimeNext = m_lTimeNext - 60 + 100;	// MMDDHHMM
	}
	return;
}

BOOL CAPDataItem::RemoveAllData()
{
	if(m_pApQurier)
	{
//		m_pApQurier->OnCodeUnAdvise(this);
	}
	if(!m_bIsExternaRawData)
	{
		m_pSdataItemDate->Init("DATE",NULL,0);
		m_pDataItemOpen->Init("OPEN",NumericSeries,0,0);
		m_pDataItemHigh->Init("HIGH",NumericSeries,0,0);
		m_pDataItemLow->Init("LOW",NumericSeries,0,0);
		m_pDataItemClose->Init("CLOSE",NumericSeries,0,0);
		m_pDataItemVolume->Init("VOLUME",NumericSeries,0,0);
		// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
		m_pDataItemExt->Init(EXECUTE_EXTDATA,NumericSeries,0,0);
	}
	return TRUE;
}

CString CAPDataItem::GetDate(int nIndex)
{
	return m_pSdataItemDate->GetAt(nIndex);
}

double CAPDataItem::GetOpenData(int nIndex)
{
	return m_pDataItemOpen->GetAt(nIndex);
}

// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
double CAPDataItem::GetExtData(int nIndex)
{
	if(m_pDataItemExt)
		return m_pDataItemExt->GetAt(nIndex);
	else 
		return 0.;
}

double CAPDataItem::GetHighData(int nIndex)
{
	return m_pDataItemHigh->GetAt(nIndex);
}

double CAPDataItem::GetLowData(int nIndex)
{
	return m_pDataItemLow->GetAt(nIndex);
}

double CAPDataItem::GetCloseData(int nIndex)
{
	return m_pDataItemClose->GetAt(nIndex);
}

double CAPDataItem::GetAmountData(int nIndex)
{
	return m_pDataItemClose->GetAt(nIndex)*m_pDataItemVolume->GetAt(nIndex);
}

double CAPDataItem::GetVolumeData(int nIndex)
{
	return m_pDataItemVolume->GetAt(nIndex);
}

/*
CString CAPDataItem::GetDayTimeData(int nIndex,int nCurrentPos)
{
	CString strWantedTime;
	CString strCurrentTime = m_pSdataItemDate->GetAt(nCurrentPos);
	strCurrentTime.Delete(strCurrentTime.GetLength()-4,4);
	CString strTimeInLoopBase;
	CString strTimeInLoop;
	int nIndexInLoop = 0;
	strTimeInLoopBase = strCurrentTime;
	if(nIndex)
	{
		strWantedTime = "";
		for(int nPos=(nCurrentPos-1);nPos>=0;nPos--)
		{
			strTimeInLoop = m_pSdataItemDate->GetAt(nPos);
			strTimeInLoop.Delete(strTimeInLoop.GetLength()-4,4);
			if(strTimeInLoopBase!=strTimeInLoop)
			{
				strTimeInLoopBase = strTimeInLoop;
				nIndexInLoop++;
			}			
			if(nIndexInLoop==nIndex)
			{
				strWantedTime = strTimeInLoopBase;
				break;
			}
		}
	}
	else
	{
		strWantedTime = strCurrentTime;
	}
	return strWantedTime;
}

double CAPDataItem::GetDayOpenData(int nIndex,int nCurrentPos)
{
	if(m_strTimeFormat=="MMDDHHMM")
	{
		CString strWantedTime = GetDayTimeData( nIndex, nCurrentPos);
		if(strWantedTime==m_strDayTime)
		{
			return m_dDayOpen;
		}
		else
		{
			m_strDayTime = strWantedTime;
			CString strTime;
			for(int nPos=(nCurrentPos);nPos>=0;nPos--)
			{
				strTime = m_pSdataItemDate->GetAt(nPos);
				strTime.Delete(strTime.GetLength()-4,4);
				if(m_strDayTime==strTime)
				{
					m_dDayOpen = m_pDataItemOpen->GetAt(nPos);
				}
			}
			return m_dDayOpen;
		}
	}
	else
	{
		return m_pDataItemOpen->GetAt(nIndex);
	}
}

double CAPDataItem::GetDayHighData(int nIndex,int nCurrentPos)
{
	if(m_strTimeFormat=="MMDDHHMM")
	{
		CString strWantedTime = GetDayTimeData( nIndex, nCurrentPos);
		if(strWantedTime==m_strDayTime)
		{
			return m_dDayHigh;
		}
		else
		{
			m_strDayTime = strWantedTime;
			CString strTime;
			double dDayHighInLoop = 0;
			double dDayHighInLoopBase = 0;
			for(int nPos=(nCurrentPos);nPos>=0;nPos--)
			{
				strTime = m_pSdataItemDate->GetAt(nPos);
				strTime.Delete(strTime.GetLength()-4,4);
				if(m_strDayTime==strTime)
				{					
					dDayHighInLoop = m_pDataItemOpen->GetAt(nPos);
					if(dDayHighInLoop>dDayHighInLoopBase)
					{
						dDayHighInLoopBase = dDayHighInLoop;
					}
				}
			}
			m_dDayHigh = dDayHighInLoopBase;
			return m_dDayHigh;
		}
	}
	else
	{
		return m_pDataItemHigh->GetAt(nIndex);
	}
	
}

double CAPDataItem::GetDayLowData(int nIndex,int nCurrentPos)
{
	if(m_strTimeFormat=="MMDDHHMM")
	{
		CString strWantedTime = GetDayTimeData( nIndex, nCurrentPos);
		if(strWantedTime==m_strDayTime)
		{
			return m_dDayLow;
		}
		else
		{
			m_strDayTime = strWantedTime;
			CString strTime;
			double dDayLowInLoop = 0;
			double dDayLowInLoopBase = 9999999999;
			for(int nPos=(nCurrentPos);nPos>=0;nPos--)
			{
				strTime = m_pSdataItemDate->GetAt(nPos);
				strTime.Delete(strTime.GetLength()-4,4);
				if(m_strDayTime==strTime)
				{					
					dDayLowInLoop = m_pDataItemOpen->GetAt(nPos);
					if(dDayLowInLoop<dDayLowInLoopBase)
					{
						dDayLowInLoopBase = dDayLowInLoop;
					}
				}
			}
			m_dDayLow = dDayLowInLoopBase;
			return m_dDayLow;
		}
	}
	else
	{
		return m_pDataItemLow->GetAt(nIndex);
	}
}

double CAPDataItem::GetDayCloseData(int nIndex,int nCurrentPos)
{
	if(m_strTimeFormat=="MMDDHHMM")
	{
		CString strWantedTime = GetDayTimeData( nIndex, nCurrentPos);
		if(strWantedTime==m_strDayTime)
		{
			return m_dDayClose;
		}
		else
		{
			m_strDayTime = strWantedTime;
			CString strTime;
			for(int nPos=(nCurrentPos);nPos>=0;nPos--)
			{
				strTime = m_pSdataItemDate->GetAt(nPos);
				strTime.Delete(strTime.GetLength()-4,4);
				if(m_strDayTime==strTime)
				{
					m_dDayClose = m_pDataItemOpen->GetAt(nPos);
				}
			}
			return m_dDayClose;
		}
	}
	else
	{
		return m_pDataItemClose->GetAt(nIndex);
	}
}
*/

long CAPDataItem::GetHHMMSSInCeilingType(LPCTSTR lpOrgData)
{
	CString strOrgData = lpOrgData;
	int nHour = 0, nMinute = 0, nSecond = 0, nSecond2 = 0;
	long lLength = strOrgData.GetLength();
	lLength-= 2;
	nSecond2 = atol(strOrgData.Mid(lLength,2));	
	lLength-= 2;
	nSecond = atol(strOrgData.Mid(lLength,2));
	lLength-= 2;
	nMinute = atol(strOrgData.Mid(lLength,2));
	nHour = atol(strOrgData.Left(lLength));

	if(nSecond2>=0)
	{
		nSecond2 = 0;
		nSecond++;
	}
	// 60�� �̻��� ��� ������ �ø�.
	if(nSecond >= 60)
	{
		nSecond = 0;
		nMinute++;

		// ���� 60�� �̻��� ��� -> �ð����� �ø�
		if(nMinute >= 60)
		{
			nMinute = 0;
			nHour++;
			// real �̱� ������ �ð��� 24�� ����� ���� ���ٰ� ����.
		}
	}

//	CString strMessage;
//	strMessage.Format("[gm0604] Original = %s,Convertid = %d",lpOrgData,(nHour*100+nMinute));
//	::OutputDebugString(strMessage);

	return (nHour*10000+nMinute*100+nSecond);
}

long CAPDataItem::GetHHMMInCeilingType(LPCTSTR lpOrgData)
{
	CString strOrgData = lpOrgData;
	int nHour = 0, nMinute = 0, nSecond = 0, nSecond2 = 0;
	long lLength = strOrgData.GetLength();
	lLength-= 2;
	nSecond2 = atol(strOrgData.Mid(lLength,2));	
	lLength-= 2;
	nSecond = atol(strOrgData.Mid(lLength,2));
	lLength-= 2;
	nMinute = atol(strOrgData.Mid(lLength,2));
	nHour = atol(strOrgData.Left(lLength));

	if(nSecond2>=0)
	{
		nSecond2 = 0;
		nSecond++;
	}
	// 01�� �̻��� ��� ������ �ø�.
	if(nSecond >= 0)
	{
		nSecond = 0;
		nMinute++;

		// ���� 60�� �̻��� ��� -> �ð����� �ø�
		if(nMinute >= 60)
		{
			nMinute = 0;
			nHour++;
			// real �̱� ������ �ð��� 24�� ����� ���� ���ٰ� ����.
		}
	}

//	CString strMessage;
//	strMessage.Format("[gm0604] Original = %s,Convertid = %d",lpOrgData,(nHour*100+nMinute));
//	::OutputDebugString(strMessage);

	return (nHour*100+nMinute);
}


/*
BOOL CAPDataItem::OnRealData(CDrMLib_CRealData *pData)
{
	CString strCode;
	pData->GetDataStr(RDI_CODE, strCode);
	if(m_strCode!=strCode)	return 0;		
	CString strTime;
	long lData = 0;
	double dOpen = 0.;
	double dHigh = 0.;
	double dLow = 0.;
	double dClose = 0.;
	double dVolume = 0.;
	double dUnit = 1.;
	if(m_strCode.GetLength()==FUTUREINDEXLENGTH)
	{
		dUnit = 0.01;
	}
	pData->GetDataStr(RDI_TIME, strTime); //�ð�
//	pData->GetDataLong(RDI_OPENPRICE,lData);
//	dOpen = (double)lData*dUnit;
//	pData->GetDataLong(RDI_HIGH,lData);
//	dHigh = (double)lData*dUnit;
//	pData->GetDataLong(RDI_LOW,lData);
//	dLow = (double)lData*dUnit;
	pData->GetDataLong(RDI_PRICE,lData);
	dClose = (double)lData*dUnit;
	pData->GetDataLong(RDI_TRDVOL,lData);
	dVolume = (double)lData;
	////
	return SetRealData(strTime, dClose, dVolume);
}
*/

BOOL CAPDataItem::SetRealData(LPCTSTR lpTime, double dClose, double dVolume)
{
	//::OutputDebugString(strTimeFromDrds);
	// ��������(6666), �帶������(7777), �帶��(8888), �ð���(9999) �˻�
	BOOL bRetValue = TRUE;
	CSTController* pSTController = NULL;
	char szData[512];
	double dCurrentPrice = 0;
	long lSize = m_pSdataItemDate->GetSize();
	if(!lSize) return 0;
	long lPos = lSize - 1;
	if(m_chType=='0')	// tick
	{
		long lToday = atol(m_pApQurier->GetDateOfRealTimeData());

//		long lTimeForTypeMinute = GetHHMMInCeilingType(lpTime) + lToday%10000*10000;// MMDDHHMM
//		sprintf(szData,"%08ld",lTimeForTypeMinute);
		long lTimeForTypeMinute = GetHHMMSSInCeilingType(lpTime) + lToday%100*1000000;// DDHHMMSS
		sprintf(szData,"%08ld",lTimeForTypeMinute);

		if(m_lCntOfDrdsForTickNumber)	// Update
		{
			m_pSdataItemDate->SetAt(lPos,szData);	// HHMMSS
			dCurrentPrice = dClose;
			m_pDataItemClose->SetAt(lPos,dCurrentPrice);
			if(dCurrentPrice>m_pDataItemHigh->GetAt(lPos))
			{
				m_pDataItemHigh->SetAt(lPos,dCurrentPrice);
			}
			if(dCurrentPrice<m_pDataItemLow->GetAt(lPos))
			{
				m_pDataItemLow->SetAt(lPos,dCurrentPrice);
			}		
			m_pDataItemVolume->SetAt(lPos,m_pDataItemVolume->GetAt(lPos) + dVolume);

			POSITION pos = m_listSTController.GetHeadPosition();
			while(pos)
			{
				pSTController = m_listSTController.GetNext(pos);
				pSTController->OnReceiveQuoteDataInRealTime(lPos, TRUE);			
			}

		}
		else							// Insert
		{
			lPos = m_pSdataItemDate->Add(szData);
			dCurrentPrice = dClose;
			m_pDataItemOpen->Add(dCurrentPrice);
			m_pDataItemHigh->Add(dCurrentPrice);
			m_pDataItemLow->Add(dCurrentPrice);
			m_pDataItemClose->Add(dCurrentPrice);
			m_pDataItemVolume->Add(dVolume);

			POSITION pos = m_listSTController.GetHeadPosition();
			while(pos)
			{
				pSTController = m_listSTController.GetNext(pos);
				pSTController->SetSizeDataItem(lPos+1);
				pSTController->OnReceiveQuoteDataInRealTime(lPos, FALSE);
			}
		}		
		m_lCntOfDrdsForTickNumber++;		
		if(m_lCntOfDrdsForTickNumber==m_lTickMinNumber)
		{
			m_lCntOfDrdsForTickNumber = 0;
		}
	}
	else if(m_chType=='1')	// minute
	{	
		long lToday = atol(m_pApQurier->GetDateOfRealTimeData());
		long lTimeForTypeMinute = GetHHMMInCeilingType(lpTime) + lToday%10000*10000;// MMDDHHMM
		sprintf(szData,"%08ld",lTimeForTypeMinute);

		if(lSize&&lTimeForTypeMinute<=m_lTimeNext)	// Update
		{
			m_pSdataItemDate->SetAt(lPos,szData);	// HHMMSS
			dCurrentPrice = dClose;
			m_pDataItemClose->SetAt(lPos,dCurrentPrice);

			if(dCurrentPrice>m_pDataItemHigh->GetAt(lPos))
			{
				m_pDataItemHigh->SetAt(lPos,dCurrentPrice);
			}
			if(dCurrentPrice<m_pDataItemLow->GetAt(lPos))
			{
				m_pDataItemLow->SetAt(lPos,dCurrentPrice);
			}		
			m_pDataItemVolume->SetAt(lPos,m_pDataItemVolume->GetAt(lPos) + dVolume);
			POSITION pos = m_listSTController.GetHeadPosition();
			while(pos)
			{
				pSTController = m_listSTController.GetNext(pos);
				pSTController->OnReceiveQuoteDataInRealTime(lPos, TRUE);
			}
		}
		else									// Insert
		{
			CString strCurrentTime;
			CString strTimeNext;
			strTimeNext.Format("%d",m_lTimeNext);
			m_pSdataItemDate->SetAt(lPos,strTimeNext);

			strCurrentTime = szData;
			lPos = m_pSdataItemDate->Add(szData);
			dCurrentPrice = dClose;
			m_pDataItemOpen->Add(dCurrentPrice);
			m_pDataItemHigh->Add(dCurrentPrice);
			m_pDataItemLow->Add(dCurrentPrice);
			m_pDataItemClose->Add(dCurrentPrice);
			m_pDataItemVolume->Add(dVolume);

			POSITION pos = m_listSTController.GetHeadPosition();
			while(pos)
			{
				pSTController = m_listSTController.GetNext(pos);
				pSTController->SetSizeDataItem(lPos+1);
				pSTController->OnReceiveQuoteDataInRealTime(lPos, FALSE);
			}
			SetTimeNext(strCurrentTime);
		}
	}
	else if(m_chType=='2')	// day
	{		
		memcpy(&szData[0],(LPCTSTR)m_pApQurier->GetDateOfRealTimeData(),8);
		szData[8] = NULL;		
		long lToday = atol(m_pApQurier->GetDateOfRealTimeData());
		if(lSize&&atol(m_pSdataItemDate->GetAt(lPos))==lToday)		// Update
		{
			if(dClose>m_pDataItemHigh->GetAt(lPos))
			{
				m_pDataItemHigh->SetAt(lPos,dClose);
			}
			if(dClose<m_pDataItemLow->GetAt(lPos))
			{
				m_pDataItemLow->SetAt(lPos,dClose);
			}
			m_pDataItemClose->SetAt(lPos,dClose);
			m_pDataItemVolume->SetAt(lPos,m_pDataItemVolume->GetAt(lPos)+dVolume);
			POSITION pos = m_listSTController.GetHeadPosition();
			while(pos)
			{
				pSTController = m_listSTController.GetNext(pos);
				pSTController->OnReceiveQuoteDataInRealTime(lPos, TRUE);
			}
			
		}
		else												// Insert
		{
			lPos = m_pSdataItemDate->Add(szData);
			m_pDataItemOpen->Add(dClose);
			m_pDataItemHigh->Add(dClose);
			m_pDataItemLow->Add(dClose);
			m_pDataItemClose->Add(dClose);
			m_pDataItemVolume->Add(dVolume);

			POSITION pos = m_listSTController.GetHeadPosition();
			while(pos)
			{
				pSTController = m_listSTController.GetNext(pos);
				pSTController->SetSizeDataItem(lPos+1);
				pSTController->OnReceiveQuoteDataInRealTime(lSize, FALSE);				
			}
		}
	}	
	return bRetValue;
}


CIDataItem* CAPDataItem::GetOpenData()
{
	return m_pDataItemOpen;
}

// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
CIDataItem* CAPDataItem::GetExtData()
{
	return m_pDataItemExt;
}

CIDataItem* CAPDataItem::GetHighData()
{
	return m_pDataItemHigh;
}

CIDataItem* CAPDataItem::GetLowData()
{
	return m_pDataItemLow;
}

CIDataItem* CAPDataItem::GetCloseData()
{
	return m_pDataItemClose;
}

CIDataItem* CAPDataItem::GetVolumeData()
{
	return m_pDataItemVolume;
}

CISDataItem* CAPDataItem::GetDateData()
{
	return m_pSdataItemDate;
}


void CAPDataItem::AddReceiver(CSTController *pSTController)
{
	POSITION pos = m_listSTController.Find(pSTController);
	if(!pos)
	{
		m_listSTController.AddTail(pSTController);
	}
}

long CAPDataItem::RemoveReceiver(CSTController *pSTController)
{
	int nCount = m_listSTController.GetCount();
	if(nCount < 0)
		return FALSE;
	POSITION pos = m_listSTController.Find(pSTController);
	if(pos)
	{
		m_listSTController.RemoveAt(pos);
	}
	return nCount;
}


long CAPDataItem::GetVarInterval()
{
	if(m_chType=='2'||m_chType=='3'||m_chType=='4')
	{
		return 1;
	}
	else
	{
		return m_lTickMinNumber;
	}	
}