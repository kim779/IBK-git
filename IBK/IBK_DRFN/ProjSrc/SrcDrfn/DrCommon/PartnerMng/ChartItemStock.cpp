// ChartItemStock.cpp: implementation of the CChartItemStock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemStock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowField[]	= { "2302", "2034", "2023", "2029", "2030", "2031", "2032", "2028", NULL };
char* szSecRowField[]	= { "6302", "6034", "6023", "6029", "6030", "6031", "6032", "6028", NULL };
char* szMinRowField[]	= { "4302", "4034", "4023", "4029", "4030", "4031", "4032", "1028", NULL };
char* szDayRowField[]	= { "5302", "5023", "5029", "5030", "5031", "5027", "5028", "5311", "5024", NULL };
char* szWeekRowField[]	= { "5302", "5023", "5029", "5030", "5031", "5027", "5028", NULL };
char* szYearRowField[]	= { "5302", "5023", "5029", "5030", "5031", "5027", "5028", NULL };

// ��������("2320") -> ���ذ�("1336") ������ ����.
//					            �����ڵ�, �̸�, ���簡, ��������, ����, �����, �����ŷ���, ���ϰŷ���
//char* szStockGridSymbol[] = { "1301", "1022", "2023", "2320", "2024", "2033", "2027", "2321",
char* szStockGridSymbol[] = { "1301", "1022", "1023", "1336", "1024", "1033", "1027", "1314",
//			���Ѱ�,���Ѱ� ,  �ð�,  ��,  ����,  ���Ͻð�, ���ϰ�, ��������,
			"1311", "1312", "1029", "1030", "1031", "1320", "1321", "1322",
			//������, ��������۽ð�, �������ð�����(HH::MM), �����ŷ����(��)
			"1307"	, "1900"		, "1901"					, "1028",			NULL};

CChartItemStock::CChartItemStock()
{
	m_bSet = FALSE;
	m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;	m_nWeekCnt = 0;	m_nYearCnt = 0; m_nGridCnt=0;
	while (szTickRowField[m_nTickCnt] != NULL)		m_nTickCnt++;
	while (szSecRowField[m_nSecCnt] != NULL)		m_nSecCnt++;
	while (szMinRowField[m_nMinCnt] != NULL)		m_nMinCnt++;
	while (szDayRowField[m_nDayCnt] != NULL)		m_nDayCnt++;
	while (szWeekRowField[m_nWeekCnt] != NULL)		m_nWeekCnt++;
	while (szYearRowField[m_nYearCnt] != NULL)		m_nYearCnt++;
	while (szStockGridSymbol[m_nGridCnt] != NULL)	m_nGridCnt++;
}

CChartItemStock::~CChartItemStock()
{
}

void CChartItemStock::SetInData(KB_p0602_InRec1* pData)
{
	try
	{
		//if(gDebugMng) gDebugMng->TraceTran('I', "SetInit", sizeof(KB_p0602_InRec1), (LPBYTE)pData);
		if(g_pDRDebuger) g_pDRDebuger->DRLog_RequestData("SetInit", (LPBYTE)pData, sizeof(KB_p0602_InRec1));
		int i = sizeof(KB_p0602_InRec1);
		memcpy(&m_InData, pData, sizeof(KB_p0602_InRec1));
		m_bSet = TRUE;
	}
	catch (CException* e)
	{
		char aBuff[MAX_PATH]={0,};
		e->GetErrorMessage(aBuff, MAX_PATH);
	}
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemStock::Convert()
{
	if(!m_bSet) 
		return "";

	CString strData, strCode, strTemp, strDate, strNow, strUnit, strTime, strImgRgb;
	int i, nCount, nUnit, nType, nFlag;
	double dUSDollar=1;

	strCode	= CDataConverter::Char2String(m_InData.shcode, sizeof(m_InData.shcode));
	strDate	= CDataConverter::Char2String(m_InData.date, sizeof(m_InData.date));
	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	strTime	= CDataConverter::Char2String(m_InData.Itime, sizeof(m_InData.Itime));
	nCount	= CDataConverter::Char2Int(m_InData.num, sizeof(m_InData.num));
	nUnit	= CDataConverter::Char2Int(m_InData.unit, sizeof(m_InData.unit));
	nType	= CDataConverter::Char2Int(m_InData.button, sizeof(m_InData.button));	// ƽ(0)/��(1)/��(2)/��(3)/��(4)
	strImgRgb = CDataConverter::Char2String(m_InData.ImgRgb, sizeof(m_InData.ImgRgb));
	nFlag     = CDataConverter::Char2Int(m_InData.nflag, sizeof(m_InData.nflag));	// 0:
	//strDate = "20120531";

	strUnit.Format(_T("%d"), nUnit);
	
	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data

	//	�����ڵ� Input �ɺ�
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	strData += "1301";	strData += sDS;	strData += strCode;	strData += sCELL;//KHD sFS;
	CString stRowData;
	CString stNextData, stNextKey, stNextKey2;

	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.nkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}
	//ȯ��
	strData += "51305"; strData += sDS;	strData += "USDKRWSMBS";	strData += sCELL; //�Է� OOP 
	//END 
	//////////////////////////////////////////////////////////////////////////

	stNextData.Format("%28.28s", m_InData.nkey);

	switch (nType)
	{
	case 0:		// 
			Datah.dindex = GI_TICK;
			Datah.dunit = GI_TICK;
			stRowData = _T("?2500");
			stNextKey = "2324";
			// 		// ���ϸ���ȸ Input �ɺ� (1:����) 
			strData += "2309";	strData += sDS;	strData += strNow;	strData += sCELL;
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
// 		//	������Ʈ ��ȸ �ð�
 		if (m_InData.Irecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.nDayBefore, sizeof(m_InData.nDayBefore));	// 0:
			CString stBefore;
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
 			strData += "2341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
//END
		break;

	case 1:		// ��
			Datah.dindex = GI_MINUTE;
			Datah.dunit = GI_MINUTE;
			stRowData = _T("?4500");
		
//KHD
		stNextKey = "4324";
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
// 		// ��/��� ���� (0:���, 1:�Ǻ�)
 		strData += "4308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		// 		// ���ϸ���ȸ Input �ɺ� (1:����) 
 		strData += "4309";	strData += sDS;	strData += strNow;	strData += sCELL;

		// �����ְ�
		strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
		strData += "4339";	strData += sDS;	strData += strTemp;	strData += sCELL;

		if (m_InData.Irecordgb[0] == '1')	//	������Ʈ ��ȸ �ð�
 		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.nDayBefore, sizeof(m_InData.nDayBefore));	// 0:
			CString stBefore;
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
 			strData += "4341";	strData += sDS;	strData += strTime;	strData += sCELL;
 		}
//END
		break;

	case 2:		// ��
			Datah.dindex = GI_DAY;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
			//KHD : �����ְ�
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;
			
		break;
	case 3:		// ��
			Datah.dindex = GI_WEEK;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
			//KHD : �����ְ�
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;
			
		break;
	case 4:		// ��
			Datah.dindex = GI_MONTH;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
			//KHD : �����ְ�
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;

		break;

	case 5:		// ��
			Datah.dindex = GI_YEAR;
			 nUnit = 1;
			stRowData = _T("?5500");
			stNextKey = "5324";
			strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
			//KHD : �����ְ�
			strTemp = CString(m_InData.modGb, sizeof(m_InData.modGb));
			strData += "5339";	strData += sDS;	strData += strTemp;	strData += sCELL;

		break;

	case 6:		// ��
		Datah.dindex = GI_SEC;
		Datah.dunit = GI_SEC;
		stRowData = _T("?6500");
	
		stNextKey = "6324";
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL;	// �ؽ�Ʈ Ű�� : KHD 
 		strData += "6308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;		// ��/��� ���� (0:���, 1:�Ǻ�)
 		strData += "6309";	strData += sDS;	strData += strNow;	strData += sCELL;			// ���ϸ���ȸ Input �ɺ� (1:����)

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	//	OutBound Data
	for (i=0; i<m_nGridCnt; i++)
	{
		strData += szStockGridSymbol[i];
		strData += sCELL;//KHD sFS;
	}
	strData += stNextKey;
	strData += sCELL;
//ȯ�� ������ 
	strData += "51331";
	strData += sCELL;

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
	memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
 	CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : �ǳ��� 0x00���ڸ� ���� �ִ´�.
	strData += szData_i;

	switch (nType)
	{
	case 0:		// ƽ
		for (i = 0; i < m_nTickCnt; i++)
		{
			strData += szTickRowField[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		for(i = 0; i < m_nMinCnt; i++)
		{
			strData += szMinRowField[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
		for(i = 0; i < m_nDayCnt; i++)
		{
			strData += szDayRowField[i];
			strData += sCOL;
		}
		break;
	case 3:		// ��
	case 4:		// ��
		for(i = 0; i < m_nWeekCnt; i++)
		{
			strData += szWeekRowField[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		for(i = 0; i < m_nSecCnt; i++)
		{
			strData += szSecRowField[i];
			strData += sCOL;
		}
		break;
	case 5:		// ��
		for(i = 0; i < m_nYearCnt; i++)
		{
			strData += szYearRowField[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}

char* CChartItemStock::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0602_OutRec1_EX stOut;
	CString strSrc = (char*)pData;
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume, strPreVolume;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit, strWonDollar;
	CString strPreHigh, strPreLow, strPreOpen;
	CString strBaseTime, strMarketTime;
	int nPrice;
	double dChRate=0;

	memset(&stOut, 0x20, sizeof(stOut));

	strCode = CDataConverter::Parser(strSrc, sCELL); // �����ڵ�

	strNode = CDataConverter::Parser(strSrc, sCELL); // �����

	//100120-alzioyes. ���úη� ������ ���� ����.
	//strNode = strNode.Mid(1);
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // ���簡
	nPrice = atoi(strPrice);
	strPrice.Remove(_T('+')); strPrice.Remove(_T('-'));
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

	
	strPrePrice = CDataConverter::Parser(strSrc, sCELL); // ��������
	memcpy(stOut.preprice, strPrePrice, min(sizeof(stOut.preprice), strPrePrice.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // ��������
	strNode = strNode.Mid(1);
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	dChRate = atof(strNode);
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/
	strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // �ŷ���
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	strPreVolume = CDataConverter::Parser(strSrc, sCELL); // ���ϰŷ���

	// Make sign value
	int nHigh, nLow;
	strHighLimit = CDataConverter::Parser(strSrc, sCELL); // ���Ѱ�
	nHigh = atoi(strHighLimit);

	strLowLimit = CDataConverter::Parser(strSrc, sCELL); // ���Ѱ�
	nLow = atoi(strLowLimit);

	theApp.DoMapStrObj_UpDnInfo(strCode, strHighLimit, strLowLimit);

	if(abs(nPrice) == nHigh)		stOut.sign[0] = '1'; // UP_LIMIT
	else if(abs(nPrice) == nLow)	stOut.sign[0] = '4'; // DOWN_LIMIT
	else if(dChRate < 0)			stOut.sign[0] = '5'; // DOWN
	else if(dChRate == 0)			stOut.sign[0] = '3'; // SAME (BOHAP)
	else							stOut.sign[0] = '2'; // UP


	strOpen = CDataConverter::Parser(strSrc, sCELL); // �ð�
	strOpen.Remove(_T('+')); strOpen.Remove(_T('-'));
	strHigh = CDataConverter::Parser(strSrc, sCELL); // ��
	strHigh.Remove(_T('+')); strHigh.Remove(_T('-'));
	strLow = CDataConverter::Parser(strSrc, sCELL); // ����
	strLow.Remove(_T('+')); strLow.Remove(_T('-'));

	strPreOpen = CDataConverter::Parser(strSrc, sCELL);		// ���Ͻð�
	strPreOpen.Remove(_T('+')); strPreOpen.Remove(_T('-'));
	strPreHigh = CDataConverter::Parser(strSrc, sCELL);		// ���ϰ�
	strPreHigh.Remove(_T('+')); strPreHigh.Remove(_T('-'));
	strPreLow = CDataConverter::Parser(strSrc, sCELL);		// ��������
	strPreLow.Remove(_T('+')); strPreLow.Remove(_T('-'));

	CString strRealState = CDataConverter::Parser(strSrc, sCELL);	
	/* 0:���� */
	/* 2:�帶�� */
	/* 3:�帶�� */
	/* 4:�ŷ����� */
	/* 5:�ŷ��簳 */
	/* 6:�������Ľð��������ŸŰ��� */
	/* 7:�������Ľð��������ŸŹ� */
	/* �ð��ܴ��ϰ��ŸŰ��� */
	/* 8:CB�ߵ� 9:CB�ߵ����� */
	/* 10:�尳���� �ð��ܸŸŰ��� */
	/* 11:�尳���� �ð��ܸŸ����� */
	/* 12:�ð��������� */
	/* 13:�ð��������� */
	/* 14:������������ */
	/* 15:������������ */
	/* 16:�ð��ܴ��ϰ�RANDOMEND���� */
	/* 17:�ð��ܴ��ϰ�RANDOMEND���� */
	/* 18:�ð��ܴ��ϰ��ŸŸ��� */
	/* 19:�ӽ����� 20:�ӽ��������簳*/
	/* 21:�ð��ܴ��ϰ� ���� */
	/* 22:����ü�ᰡ���� ���� */

//	CString strRequestTime = CDataConverter::Parser(strSrc, sCELL);	// "1390" ������ð�.
//	CString strMarketStartTime = CDataConverter::Parser(strSrc, sFS); // "1389" ��������۽ð�(HHMM)
//	CString strMarketStartTime = CDataConverter::Parser(strSrc, sFS); // "1600" ����� �ڵ�(������/�ð��ܴ��ϰ���..)
// 	if(strMarketStartTime.GetLength() == 4) // "0900" -> "090000"
// 		strMarketStartTime += "00";

	//���ð��� üũ�ؼ� ���� ���������� �����޽������� ��ð��� �������� �ʵ��� �Ѵ�.
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL); //"1391" ���ð�����(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL); //"1392" ���ð�����(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();

	BOOL bSetReal = FALSE;
	if(strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0 || strRealState.Compare("22") == 0)
		bSetReal = TRUE;

	if(strJangStart.GetLength() == 4)	strJangStart += "00";

	//>>0001067: [��Ʈ] ����� �ð��� 9�ð� �ƴ� ���(ex : ����) N�к��� ����� �ð��������� �����ǰ� �������� ����
	//������ ��� 9�ð� �ƴ� ��쿡 �����ϹǷ� ����������� �ٲ��� ��.
	//BASETIME(HHMMSS)�� MARKETTIME(HHMMSS|HHMMSS) �Ѵ� ���õǾ�� ��.
	//if(strJangStart.Compare("090000") != 0)
	CString strResetTime = "";
	if(bSetReal)
	{
		strResetTime = strJangStart;
	}
	strBaseTime = strJangStart;
	strMarketTime.Format("%4.4s00|%4.4s00", strJangStart, strJangEnd);
	//<<

	//if(strJangStart.Compare("0000")!=0)		strMarketStartTime.Empty();

	//[A00000651][A00000652]�����ŷ����.
	CString strVolAmt = CDataConverter::Parser(strSrc, sCELL); //"2028" �����ŷ����
	strVolAmt.TrimRight();
	strVolAmt += _T("000000");

	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);
	//KHD : 
	CString strUSDollor = CDataConverter::Parser(strSrc, sCELL);
	memcpy(m_InData.usRate, (LPTSTR)(LPCTSTR)strUSDollor, min(strUSDollor.GetLength(), sizeof(m_InData.usRate)));
	
	int nSize, nCnt, nType;
	strNode = CString(m_InData.button, sizeof(m_InData.button)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	nType = atoi(strNode);
	if (nType == 0)			nSize = m_nTickCnt;	// ƽ
	else if (nType == 1)	nSize = m_nMinCnt;	// ��
	else if (nType == 6)	nSize = m_nSecCnt;	// ��
	else if (nType == 2)	nSize = m_nDayCnt;	// ��
	else if (nType == 5)	nSize = m_nYearCnt;	// ��
	else					nSize = m_nWeekCnt;	// ��, ��

//	����� �������ϱ�.	
// 	CString szTTT;
//	szTTT = CDataConverter::Parser(strSrc, sFS); // 17408 ����� �������ϱ�.

	// �׸��� ���
	//GRAPH_IO gridOut;
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));
	
	if (strNextKey =="99999999999999" || atoi(strNextKey) == 0)// 9�� 14�ڸ��� ����Ű�� ���°�.  
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
	//END

	CStringArray arrNode;
	int nNodeIndex = 1;
	strSrc.TrimLeft(); strSrc.TrimRight();
	strSrc+=sCELL;
	strSrc+=sFS;
	strSrc+=sROW;
	strSrc+=sCELL;
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
	
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// ��ü ������ �� ���
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// ��Ʈ �� ������
	int nBongSize = sizeof(KB_p0602_OutRec2);
	int nBufSize = sizeof(KB_p0602_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0602_OutRec1_EX);

	nDataLen = nBufSize;

	CString strDate, strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0602_OutRec2 stFirstBong, stBong;

	CStringArray arrTest;
	double dValue=0;
	int nIndex = 0;
	int nEmptyIndex = 0;
	int nRow = 0;
	for ( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);
		int Price = atoi(strDate);
		
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
		if(nType < 2 || nType == 6)	//ƽ�ʺ�.
		{			
			//	strNode += "000000";//KHD
		}
		else
		{
			// 20121024 �̻��	������ �����Ϳ� sCELL ���� �پ� �ִ� ��� .. sCELL ���� ������ �ٽ� ����. --->>>>
			if (nRow == nCnt - 1)
			{
				int nFind = strNode.Find(sCELL);
				if (nFind > 0)
					strNode = strNode.Left(nFind);
			}
			//	---------<<<<<<

			strNode += "000000";
		}

		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--

		// �Ǹ���
		if (nType == 2)
		{
			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.rights, strNode, min(sizeof(stBong.rights), strNode.GetLength()));

			strNode = arrNode.GetAt(nIndex++);
			memcpy(stBong.sign, strNode, min(sizeof(stBong.sign), strNode.GetLength()));
		}

		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		// ù �����͸� ������ ���´�.
	    if (nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	if (nCnt > 0 && nType == 3 && m_InData.nflag[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = strDate;
	else
		m_bojoInput.m_sLastDataTime = _T("");

//	nCnt-=nEmptyIndex;
	double dUSDollar=0;
	// ������ ���� : Ÿ����Ʈ�� �����ʿ��� ó��
	int nDataType = int(m_InData.nDataGubun[0] - '0'); 
	if(nDataType > 0)
	{
		CString strData(stFirstBong.close, sizeof(stFirstBong.close));
		int nFirstClose = atoi(strData);
		double dDenom; //�и�
		KB_p0602_OutRec2 stBong;
		//@Solomon	091123SM072
		double dData;
		double dUSDollar = CDataConverter::Char2Double(m_InData.usRate, sizeof(m_InData.usRate));

		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;		break;	// ù���� ����
		case 3:	dDenom = atof(strOpen);		break;	// ���Ͻð�
		case 4:	dDenom = nPrice;			break;	// ���簡
		case 5:	dDenom = atof(strPrePrice);	break;	// ��������
		case 2:
			dDenom = dUSDollar;

			dData = atof(strPrePrice)*100 / dUSDollar;
			strPrePrice.Format(_T("%ld"), (long)dData);
			dData = atof(strLowLimit)*100 / dUSDollar;
			strLowLimit.Format(_T("%ld"), (long)dData);

			dData = atof(strHighLimit)*100 / dUSDollar;
			strHighLimit.Format(_T("%ld"), (long)dData);
 ////KHD : ��ܹ� ������ ���� ����			
 		dData = atof(strOpen)*100 ;// dUSDollar;
 			strOpen.Format(_T("%ld"), (long)dData);
 			dData = atof(strHigh)*100 ;// dUSDollar;
 			strHigh.Format(_T("%ld"), (long)dData);
 			dData = atof(strLow)*100 ;// dUSDollar;
 			strLow.Format(_T("%ld"), (long)dData);
//END 
			dData = atof(strPrice)*100 / dUSDollar;
			strPrice.Format(_T("%ld"), (long)dData);

			strWonDollar.Format(_T("%ld"), (long)dUSDollar);
			break;
		}

//		if (dDenom == 0)
//			dDenom = atoi(strOpen);

		if (nDataType == 2)
		{
			for (nRow = 0; nRow < nCnt; nRow++)
			{
				memcpy(&stBong, &pDataBuf[nPosBong + nRow*nBongSize], nBongSize);
				// �ð�
				strData = CDataConverter::Get_RateData(stBong.open, dDenom, sizeof(stBong.open));
				memcpy(stBong.open, strData, min(strData.GetLength(), sizeof(stBong.open)));
				// ��
				strData = CDataConverter::Get_RateData(stBong.high, dDenom, sizeof(stBong.high));
				memcpy(stBong.high, strData, min(strData.GetLength(), sizeof(stBong.high)));
				// ����
				strData = CDataConverter::Get_RateData(stBong.low, dDenom, sizeof(stBong.low));
				memcpy(stBong.low, strData, min(strData.GetLength(), sizeof(stBong.low)));
				// ����
				strData = CDataConverter::Get_RateData(stBong.close, dDenom, sizeof(stBong.close));
				memcpy(stBong.close, strData, min(strData.GetLength(), sizeof(stBong.close)));
				
				memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
			}
		}
		else
		{
			for (nRow = 0; nRow < nCnt; nRow++)
			{
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
	}

	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ���� �޽��� �����
	CChartBozoManager bojoMng;
	m_bojoInput.m_sShcode = strCode;
	m_bojoInput.m_sPrice = strPrice;		// ���簡
	m_bojoInput.m_sVolume = strVolume;		// �ŷ���
	m_bojoInput.m_sUpLmtPrice = strHighLimit;	// ���Ѱ�
	m_bojoInput.m_sDnLmtPrice = strLowLimit;	// ���Ѱ�
	m_bojoInput.m_sOpen = strOpen;		// �ð�
	m_bojoInput.m_sHigh = strHigh;		// ��
	m_bojoInput.m_sLow = strLow;			// ����
//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen = strPreOpen;		// ���Ͻð�
	m_bojoInput.m_sPreHigh = strPreHigh;		// ���ϰ�
	m_bojoInput.m_sPreLow = strPreLow;		// ��������
	m_bojoInput.m_sWonDollor = strWonDollar;	// ���޷� ȯ��
//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice = strPrePrice;	// ��������
	m_bojoInput.m_sPreVolume = strPreVolume;	// ���ϰŷ���
	m_bojoInput.m_sStartTime = strResetTime;//"090000";	// �ŷ����۽ð�(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close));	// ��ȸ�� ù���� ����.

	m_bojoInput.m_sDataType.Format(_T("%c"), m_InData.nDataGubun[0]);
	m_bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sNxtFlg = m_InData.nflag[0];

	m_bojoInput.m_sTerm.Format("%d", nType);	//	��,��,��
	m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
	if (nType == 1)
	{
		//{{JS.Kim_20100903 Solomon 60�к� �̻��� ��� �ð����� ���� �����Ͽ��� ��
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
	}
	else if (nType >= 2 && nType <= 5)		//	��,��,��,��
		m_bojoInput.m_sTick = _T("1");

	m_bojoInput.m_sVolAmt = strVolAmt;	//[A00000651][A00000652]�����ŷ����.

	m_bojoInput.m_szMarketTime	= strMarketTime;	//������.
	m_bojoInput.m_szBaseTime	= strBaseTime;		//�к��� �� ���� ���۱��� �ð�.

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 101); // 101�� �ֽ���Ʈ
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0496", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
