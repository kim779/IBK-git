// ChartItemUpjong.cpp: implementation of the CChartItemUpjong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemUpjong.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowFieldUJ[] ={"22302", "22034", "22023", "22029", "22030", "22031", "22032", "22028", };
char* szSecRowFieldUJ[] = {"26302", "26034", "26023", "26029", "26030", "26031", "26032", "26028", };
char* szMinRowFieldUJ[] = {"24302", "24034", "24023", "24029", "24030", "24031", "24032", "24028", };
char* szDayRowFieldUJ[] = {"25302",          "25023", "25029", "25030", "25031", "25027", "25028", }; // IBK�� 25�̴�. 

CChartItemUpjong::CChartItemUpjong()
{
	m_bSet = FALSE;
}

CChartItemUpjong::~CChartItemUpjong()
{

}

void CChartItemUpjong::SetInData(KB_p0603_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0603_InRec1));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemUpjong::Convert()
{
	if(!m_bSet) 
		return "";

	CString strData, strCode, strTemp, strImgRgb, strDate;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Supcode, sizeof(m_InData.Supcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);
	strDate	= CDataConverter::Char2String(m_InData.Sdate, sizeof(m_InData.Sdate));
	strImgRgb = CDataConverter::Char2String(m_InData.ImgRgb, sizeof(m_InData.ImgRgb));
	CString strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////

	// ��Ʈ ������
	strData += "21301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData +=  sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}
	//ȯ��
	CString stRowData;
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	switch(nType)
	{
	case 0:		// ƽ
		Datah.dindex = GI_TICK;
		stRowData = _T("?22500");
	
		strData += "22309";	strData += sDS;	strData += strNow;	strData += sCELL;//����
//KHD : NEXT		
		stNextKey = "22324";
		stNextData.Format("%20.20s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
//END	
		break;

	case 1:		// ��
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?24500");
		
		stNextKey = "24324";
		stNextData.Format("%14.14s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		// ��/��� ���� (0:���, 1:�Ǻ�)
		strData += "24308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		//���ϸ� ��ȸ 
		strData += "24309"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;

		break;

	case 2:		// ��
		Datah.dindex = GI_DAY;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;
	case 3:		// ��
		Datah.dindex = GI_WEEK;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;
	case 4:		// ��
		Datah.dindex = GI_MONTH;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;
	case 5:		// ��
		Datah.dindex = GI_YEAR;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;
	case 6:		// ��
		Datah.dindex = GI_SEC;
		stRowData = _T("?26500");
		stNextKey = "26324";
		stNextData.Format("%14.14s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		// ��/��� ���� (0:���, 1:�Ǻ�)
		strData += "26308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		//���ϸ� ��ȸ 
		strData += "26309"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
						//  �����ڵ�,  �̸�,  ��������, ��������, ����,  �����, �ŷ���, ���ϰŷ���(�����ŷ����)
	char* pUpjongSymbol[] = { "21301", "21022", "21023", "21313", "21024", "21033", "21027", "21028",
						//  �ð�����, ������, ��������,  ���Ͻð�, ���ϰ�, ��������,
							"21029", "21030", "21031", "21320", "21321", "21322", "21307" ,"21900", "21901" };

	for(i = 0; i < sizeof(pUpjongSymbol)/sizeof(char*); i++)
	{
		strData += pUpjongSymbol[i];
		strData += sCELL;//KHDsFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	//////////////////////////////////////////////////////////////////////////

	// íƮ �ɺ�
	//////////////////////////////////////////////////////////////////////////
	// Row Data ����

	strData += stRowData;
	strData += sDS;
	
	CString temp;
	//��ȸ������ �� 
	int GrpHLen = 0;
	temp.Format("%06d", nCount);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = 0 + 0x30;
	
	CopyMemory(Datah.pday, strDate, strDate.GetLength());
	
	temp.Format("%0*d", sizeof(Datah.lgap), nUnit);// N�к� 
	CopyMemory(Datah.lgap, temp, temp.GetLength() );
	
	char szData_i[sz_DATAH];
	memset(szData_i, 0x00, sizeof(Datah));
	memcpy(szData_i, (char*)&Datah, sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : �ǳ��� 0x00���ڸ� ���� �ִ´�.
	strData += szData_i;

	int nSize;

	switch(nType)
	{
	case 0:		// ƽ
		nSize = sizeof(szTickRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldUJ[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = sizeof(szMinRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldUJ[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
	case 5:		// ��
		nSize = sizeof(szDayRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldUJ[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = sizeof(szSecRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldUJ[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}

char* CChartItemUpjong::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0603_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strVolume, strPreVolume;

	double dOpen, dHigh, dLow, dPreOpen, dPreHigh, dPreLow;//, dMaxHigh, dMinLow;
	double dPrePrice;

	memset(&stOut, 0x20, sizeof(stOut));

//  �����ڵ�,  �̸�,  ��������, ��������, ����,  �����, �ŷ���, ���ϰŷ���
//{ "20301", "20022", "20023", "20306", "20024", "20033", "20027", "20307",
//  �ð�����, ������, ��������,
//	"20029", "20030", "20031", };

	strCode = CDataConverter::Parser(strSrc, sCELL); // �����ڵ�

	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // ���簡
	int nPrice = atoi(strPrice);
	strPrice.Remove(_T('+')); strPrice.Remove(_T('-')); strPrice.Remove(_T('.'));
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPrePrice = fabs(atof(strNode));	// ��������
	strNode.Format(_T("%d"), dPrePrice*100);
	memcpy(stOut.preprice, strNode, min(sizeof(stOut.preprice), strNode.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // ��������
	strNode = strNode.Mid(1);
	strNode.Remove(_T('.'));
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/
	strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // �ŷ���
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	strPreVolume = CDataConverter::Parser(strSrc, sCELL); // ���ϰŷ���

	// Make sign value
	if(nPrice < 0)		stOut.sign[0] = '5'; // DOWN
	else if(abs(nPrice) == (int)(dPrePrice*100))	stOut.sign[0] = '3'; // Same (BOHAP)
	else stOut.sign[0] = '2'; // UP
	
	strNode = CDataConverter::Parser(strSrc, sCELL);	dOpen = fabs(atof(strNode));	// �ð�
	strNode = CDataConverter::Parser(strSrc, sCELL);	dHigh = fabs(atof(strNode));	// ��
	strNode = CDataConverter::Parser(strSrc, sCELL);	dLow = fabs(atof(strNode));		// ����
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreOpen = fabs(atof(strNode));	// ���Ͻð�
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreHigh = fabs(atof(strNode));	// ���ϰ�
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreLow = fabs(atof(strNode));	// ��������
	
	CString strRealState = CDataConverter::Parser(strSrc, sCELL);   //�� ����
																	// 00:�尳����                    
																	// 10:�ð����ϰ�                  
																	// 11:�ð����ϰ�����              
																	// 20:���ߴ��ϰ�                  
																	// 21:���ߴ��ϰ�����              
																	// 30:�������ϰ�                  
																	// 40:����                        
																	// 50:�尳�����ð���              
																	// 60:�������Ľð�������          
																	// 65:�������Ľð���              
																	// 70:�ð��ܴ��ϰ�                
																	// 80:�����Ÿ�ü��                
																	// 90:�ŷ�����                    
																	// 99:������              
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL);   // ���ð�����(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL);   // ���ð�����(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();
	
	BOOL bSetReal = FALSE;
	if(strRealState.Compare("00") == 0 || strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0 || strRealState.Compare("50") == 0)
		bSetReal = TRUE;
	
	if(strJangStart.GetLength() == 4)	strJangStart += "00";


	CString strResetTime = "";
	if(bSetReal)
	{
		strResetTime = strJangStart;
	}
	CString strBaseTime = strJangStart;
	CString strMarketTime;
	strMarketTime.Format("%4.4s00|%4.4s00", strJangStart, strJangEnd);

	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

	int nSize, nCnt, nType;
	strNode = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	nType = atoi(strNode);
	if(nType == 0) // ƽ
		nSize = sizeof(szTickRowFieldUJ)/sizeof(char*);
	else if(nType == 1) //��
		nSize = sizeof(szMinRowFieldUJ)/sizeof(char*);
	else if(nType == 6) //��
		nSize = sizeof(szSecRowFieldUJ)/sizeof(char*);
	else		// ��, ��, ��, ��
		nSize = sizeof(szDayRowFieldUJ)/sizeof(char*);
	
	// �׸��� ���
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));
	//091027-alzioyes.���� DRFN��Ʈ���� �Ҽ������� �����͸� �޾Ҵ�. ����.
	strSrc.Remove('.');
	
	if (strNextKey =="99999999999999" || atoi(strNextKey) == 0)// 9�� 14�ڸ��� ����Ű�� ���°�.  
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
	//END

	CStringArray arrNode;
	int nNodeIndex = 1;
	while(strSrc.IsEmpty() == FALSE)
	{	
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
	//	strNode.TrimLeft(); strNode.TrimRight();
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// ��ü ������ �� ���
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// ��Ʈ �� ������
	int nBongSize = sizeof(KB_p0603_OutRec2);
	int nBufSize = sizeof(KB_p0603_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0603_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate, strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0603_OutRec2 stFirstBong, stBong;

	double dValue=0;
	int nIndex = 0;
	int nRow = 0;
	for( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);
		
		// time : ƽ,��,���� ��
		if (nType < 2 || nType == 6)
			strTime = arrNode.GetAt(nIndex++); 
		
		if (nType == 0) // DDHHMMSS
			strNode = strDate.Right(2) + strTime;
		else if (nType == 1) //MMDDHHMM
			strNode = strDate.Right(4) + strTime.Left(4);
		else if (nType == 4) //YYYYMM
			strNode = strDate.Left(6);
		else if (nType == 6) //DDHHMMSS
			strNode = strDate.Right(2) + strTime.Left(6);
		else if (nType == 5) // ��.
		{
			if (nRow != nCnt-1)
				strNode.Format(_T("%s12"), strDate.Left(4));
			else
				strNode = strDate.Left(6);
		}
		else
			strNode = strDate;

		memcpy(stBong.date, strNode, min(sizeof(stBong.date), strNode.GetLength()));

		// ����/ü�ᰡ
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
		// �ð�
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.open, strNode, min(sizeof(stBong.open), strNode.GetLength()));
		// ��
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.high, strNode, min(sizeof(stBong.high), strNode.GetLength()));
		// ����
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.low, strNode, min(sizeof(stBong.low), strNode.GetLength()));
		// �ŷ���
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.volume, strNode, min(sizeof(stBong.volume), strNode.GetLength()));
		// �ŷ����
		//@Solomon	091202SM123	-->
		strNode = arrNode.GetAt(nIndex++);
		//dValue = atof(strNode)*1000000;
		//strNode.Format(_T("%.0f"), dValue);
		strNode += "000000";
// 		if(nRow==0)
// 		{
// 			CString szDebug;
// 			szDebug.Format("[SMalzio] UP�ŷ����=%s", strNode);
// 			OutputDebugString(szDebug);
// 		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--

		// ������ ���ۿ� ����, ���ֿ��� ��,��,ƽ�� ������ �ݴ�
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		// ù �����͸� ������ ���´�.
		if(nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	if (nCnt > 0 && nType == 3 && m_InData.Snextgb[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

	// ������ ���� : Ÿ����Ʈ�� �����ʿ��� ó��
	int nDataType = int(m_InData.SdataType[0] - '0'); 
	if(nDataType > 0)
	{
		CString strData(stFirstBong.close, sizeof(stFirstBong.close));
		int nFirstClose = atoi(strData);
		double dDenom; //�и�
		KB_p0602_OutRec2 stBong;
		
		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;		break;	// ù���� ����
		case 2:	dDenom = 1;					break;	// $  ������ ó������
		case 3:	dDenom = dOpen*100;			break;	// ���Ͻð�
		case 4:	dDenom = nPrice;			break;	// ���簡
		case 5:	dDenom = dPrePrice*100;		break;	// ��������
		}

		for(nRow = 0; nRow < nCnt; nRow++)
		{
			if(nDataType == 2) break;

			memcpy(&stBong, &pDataBuf[nPosBong + nRow*nBongSize], nBongSize);
			// �ð�
			strData = CDataConverter::Get_PercentData(stBong.open, dDenom, sizeof(stBong.open));
			memcpy(stBong.open, strData, min(strData.GetLength(), sizeof(stBong.open)));
			// ��
			strData = CDataConverter::Get_PercentData(stBong.high, dDenom, sizeof(stBong.high));
			memcpy(stBong.high, strData, min(strData.GetLength(), sizeof(stBong.high)));
			// ����
			strData = CDataConverter::Get_PercentData(stBong.low, dDenom, sizeof(stBong.low));
			memcpy(stBong.low, strData, min(strData.GetLength(), sizeof(stBong.low)));
			// ����
			strData = CDataConverter::Get_PercentData(stBong.close, dDenom, sizeof(stBong.close));
			memcpy(stBong.close, strData, min(strData.GetLength(), sizeof(stBong.close)));

			memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ���� �޽��� �����
	CChartBozoManager bojoMng;
	m_bojoInput.m_sShcode = strCode;
	m_bojoInput.m_sPrice = strPrice;		// ���簡
	m_bojoInput.m_sVolume = strVolume;		// �ŷ���
//	m_bojoInput.m_sUpLmtPrice.Format(_T("%.0f"), dMaxHigh*100);	// ���Ѱ�
//	m_bojoInput.m_sDnLmtPrice.Format(_T("%.0f"), dMinLow*100);	// ���Ѱ�
	m_bojoInput.m_sOpen.Format(_T("%.0f"), dOpen*100);			// �ð�
	m_bojoInput.m_sHigh.Format(_T("%.0f"), dHigh*100);			// ��
	m_bojoInput.m_sLow.Format(_T("%.0f"), dLow*100);			// ����
	//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen.Format(_T("%.0f"), dPreOpen*100);	// ���Ͻð�
	m_bojoInput.m_sPreHigh.Format(_T("%.0f"), dPreHigh*100);	// ���ϰ�
	m_bojoInput.m_sPreLow.Format(_T("%.0f"), dPreLow*100);		// ��������
	//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice.Format(_T("%.0f"), dPrePrice*100);	// ��������
	m_bojoInput.m_sPreVolume = strPreVolume;	// ���ϰŷ���
	m_bojoInput.m_sStartTime = strResetTime;	// �ŷ����۽ð�(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// ��ȸ�� ù���� ����.

	m_bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sNxtFlg = m_InData.Snextgb[0];

	m_bojoInput.m_sTerm.Format("%d", nType);
	m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	if (nType == 1)
	{
		//{{JS.Kim_20100903 Solomon 60�к� �̻��� ��� �ð����� ���� �����Ͽ��� ��
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
		m_bojoInput.m_szBaseTime	= strBaseTime;	
	}
	else if (nType >= 2 && nType <= 5)		//	��,��,��,��
		m_bojoInput.m_sTick = _T("1");

	m_bojoInput.m_szMarketTime	= strMarketTime;	//������.

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 20); // 102�� ����
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	//nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode, 1);

	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
