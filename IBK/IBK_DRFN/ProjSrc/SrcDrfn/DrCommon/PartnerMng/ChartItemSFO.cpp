// ChartItemSFO.cpp: implementation of the CChartItemSFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemSFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowFieldSFO[] =	{"62302", "62034", "62023", "62029", "62030", "62031", "62032", "65028","65201", };
char* szSecRowFieldSFO[] =	{"66302", "66034", "66023", "66029", "66030", "66031", "66032", "65028","65201", };
char* szMinRowFieldSFO[] =	{"64302", "64034", "64023", "64029", "64030", "64031", "64032", "65028","65201", };
char* szDayRowFieldSFO[] =	{"65302",           "65023", "65029", "65030", "65031", "65027", "65028","65201", };

CChartItemSFO::CChartItemSFO()
{
	m_bSet = FALSE;
}

CChartItemSFO::~CChartItemSFO()
{

}

void CChartItemSFO::SetInData(KB_d1013_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1013_InRec1));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemSFO::Convert()
{
	if(!m_bSet) return "";

	CString strData, strCode, strTemp, strDate, strImgRgb;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strDate	= CDataConverter::Char2String(m_InData.Sdate, sizeof(m_InData.Sdate));
	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	CString strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));

	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////

	// ��Ʈ ������
	strData += "61301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sCELL;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	CString stRowData;
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);

	switch(nType)
	{
	case 0:		// ƽ
		Datah.dindex = GI_TICK;
		stRowData = _T("?62500");
		strData += "62309";	strData += sDS;	strData += strNow;	strData += sCELL;
		//KHD : NEXT		
		stNextKey = "62324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		break;

	case 1:		// ��
		//
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?64500");	
		//KHD : NEXT		
		stNextKey = "64324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		strData += "64308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "64309"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;

		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "64339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;

		break;

	case 2:		// ��
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?65500");
		//KHD : NEXT		
		stNextKey = "65324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "65339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 3:		// ��
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?65500");
		//KHD : NEXT		
		stNextKey = "65324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "65339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 4:		// ��
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?65500");
		//KHD : NEXT		
		stNextKey = "65324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "65339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 6:		// ��
		Datah.dindex = GI_SEC;
		stRowData = _T("?66500");	
		//KHD : NEXT		
		stNextKey = "66324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		strData += "66308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "66309"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;

		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "66339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
						// �����ڵ�,  �̸�,  ���簡, ��������,����, �����, �����ŷ���
	char* pFOSymbol[] = { "61301", "61022", "61023", "61336", "61024", "61033", "61027",
						// ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,
							"61311", "61312", "61029", "61030", "61031",
						// ���Ͻð�, ���ϰ�, ��������, �����ŷ����, ���ذ�//-> IBK�� ��û
							//"61320", "61321", "61322", "61028", "61038"};
							"61320", "61321", "61322",            "61336", "61307", "61900", "61901"};
//							"30354", "30355", "30358", "30359", "30360", };

	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;
	}

	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound��
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Row Data ����
	int nSize;
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
	switch(nType)
	{
	case 0:		// ƽ
		nSize = sizeof(szTickRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldSFO[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = sizeof(szMinRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldSFO[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		nSize = sizeof(szDayRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldSFO[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = sizeof(szSecRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldSFO[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;

	return strData;
}

char* CChartItemSFO::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_d1013_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume, strVolAmt;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit;
	CString strPreHigh, strPreLow, strPreOpen, strStandard;
	double dChRate=0.0;

	memset(&stOut, 0x20, sizeof(stOut));

// �����ڵ�,  �̸�,  ���簡, ��������,����, �����, �����ŷ���
//{ "30301", "30022", "31023", "31356", "31024", "31033", "31027",
// ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,
//	"30354", "30355", "30358", "30359", "30360", };

	strCode = CDataConverter::Parser(strSrc, sCELL); // �����ڵ�

	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // ���簡
	strPrice.Remove(_T('+')); strPrice.Remove(_T('-')); strPrice.Remove(_T('.'));
	int nPrice = atoi(strPrice);
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

	strPrePrice = CDataConverter::Parser(strSrc, sCELL); // ��������
	strPrePrice.Remove(_T('.'));
	memcpy(stOut.preprice, strPrePrice, min(sizeof(stOut.preprice), strPrePrice.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // ��������
	strNode = strNode.Mid(1);
	strNode.Remove(_T('.'));
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	dChRate = atof(strNode);
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // �ŷ���
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	// Make sign value
	int nHigh, nLow;
	strHighLimit = CDataConverter::Parser(strSrc, sCELL); // ���Ѱ�
	strHighLimit.Remove(_T('.'));
	nHigh = atoi(strHighLimit);

	strLowLimit = CDataConverter::Parser(strSrc, sCELL); // ���Ѱ�
	strLowLimit.Remove(_T('.'));
	nLow = atoi(strLowLimit);

	if(abs(nPrice) == nHigh)		stOut.sign[0] = '1'; // UP_LIMIT
	else if(abs(nPrice) == nLow)	stOut.sign[0] = '4'; // DOWN_LIMIT
	else if(dChRate < 0)			stOut.sign[0] = '5'; // DOWN
	else if(dChRate == 0)			stOut.sign[0] = '3'; // SAME (BOHAP)
	else							stOut.sign[0] = '2'; // UP

	strOpen = CDataConverter::Parser(strSrc, sCELL); // �ð�
	strHigh = CDataConverter::Parser(strSrc, sCELL); // ��
	strLow = CDataConverter::Parser(strSrc, sCELL); // ����
	strOpen.Remove(_T('.'));
	strHigh.Remove(_T('.'));
	strLow.Remove(_T('.'));
 
	strPreOpen = CDataConverter::Parser(strSrc, sCELL);		// ���Ͻð�
	strPreOpen.Remove(_T('+'));	strPreOpen.Remove(_T('-'));
	strPreHigh = CDataConverter::Parser(strSrc, sCELL);		// ���ϰ�
	strPreHigh.Remove(_T('+'));	strPreHigh.Remove(_T('-'));
	strPreLow = CDataConverter::Parser(strSrc, sCELL);		// ��������
	strPreLow.Remove(_T('+'));	strPreLow.Remove(_T('-'));

	//>>[A00000651][A00000652]�����ŷ����.
//	strNode = CDataConverter::Parser(strSrc, sCELL);
//	strVolAmt = strNode;
	strVolAmt += _T("00000");
	//<<[A00000651][A00000652
	strStandard = CDataConverter::Parser(strSrc, sCELL);//���ذ� ;

	CString strRealState = CDataConverter::Parser(strSrc, sCELL);   //�ֽļ��� ������(61307)	
																	// 00 : �尳����
																	// 10 : �ð����ϰ�       
																	// 11 : �ð����ϰ�����   
																	// 20 : ���ߴ��ϰ�       
																	// 21 : ���ߴ��ϰ� ����  
																	// 30 : �������ϰ�       
																	// 40 : ����            
																	// 99 : ������           
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL);   // ���ð�����(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL);   // ���ð�����(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();
// 	{
// 		CString strDebug;
// 		strDebug.Format("[DRCS]�ֽļ��� �����ڵ�[%s] ������:[%s]", strCode, strRealState);
// 		OutputDebugString(strDebug);
// 	}

	BOOL bSetReal = FALSE;
	if(strRealState.Compare("00") == 0 || strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0)
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
		nSize = sizeof(szTickRowFieldSFO)/sizeof(char*);
	else if(nType == 1) //��
		nSize = sizeof(szMinRowFieldSFO)/sizeof(char*);
	else if(nType == 6) //��
		nSize = sizeof(szSecRowFieldSFO)/sizeof(char*);
	else		// ��, ��, ��
		nSize = sizeof(szDayRowFieldSFO)/sizeof(char*);
	
	// �׸��� ���
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));

	if (strNextKey =="99999999999999" || atoi(strNextKey) == 0)// 9�� 14�ڸ��� ����Ű�� ���°�.  
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));

	strSrc.Remove('.');
	
	CStringArray arrNode;
	int nNodeIndex = 1;
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
	int nBongSize = sizeof(KB_d1013_OutRec2);
	int nBufSize = sizeof(KB_d1013_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_d1013_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_d1013_OutRec2 stFirstBong, stBong;

	double dValue=0;
	int nIndex = 0, nRow;
	for(nRow = nCnt-1; nRow >= 0; nRow--)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);

		// time : ƽ,��,���� ��
		if(nType < 2 || nType == 6) strTime = arrNode.GetAt(nIndex++); 
		
		if(nType == 0) // DDHHMMSS
			strNode = strDate.Right(2) + strTime;
		else if(nType == 1) //MMDDHHMM
			strNode = strDate.Right(4) + strTime.Left(4);
		else if(nType == 4) //YYYYMM
			strNode = strDate.Left(6);
		else if(nType == 6) //DDHHMMSS
			strNode = strDate.Right(2) + strTime.Left(6);
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
		//dValue = atof(strNode)*100000;	//[A00000408]�ŷ���� 12 -> 18. (6 -> 5��)
		//strNode.Format(_T("%.0f"), dValue);
		strNode += "00000";
// 		if(nRow==nCnt-1)
// 		{
// 			CString szDebug;
// 			szDebug.Format("[SMalzio] SFO�ŷ����=%s", strNode);
// 			OutputDebugString(szDebug);
// 		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--
		//�̰������� 
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.openinterest, strNode, min(sizeof(stBong.openinterest), strNode.GetLength()));

		// ������ ���ۿ� ����, ���ֿ��� ��,��,ƽ�� ������ �ݴ�
		//memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
		memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);//KHD : �ݴ�� �ͼ� �ݴ�� ���� 
		if (nRow == 0)
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
		
		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose; break;	// ù���� ����
		case 2:	dDenom = 1; break;				// $   ������ ó������
		case 3:	dDenom = atof(strOpen); break;	// ���Ͻð�
		case 4:	dDenom = nPrice; break;			// ���簡
		case 5:	dDenom = atof(strPrePrice); break;		// ��������
		}

//		���������� �ȵ�� �ð�� ������������ �׷�����.
//		�׽�Ʈ�ϱ� ���� �ð��� �����Ͽ� �׷����� �Ѱ���.
//		if (dDenom == 0)
//			dDenom = atoi(strOpen);

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
	strHighLimit.Remove('-');
	strLowLimit.Remove('-');
	strLow.Remove('-');
	m_bojoInput.m_sUpLmtPrice = strHighLimit;	// ���Ѱ�
	m_bojoInput.m_sDnLmtPrice = strLowLimit;	// ���Ѱ�
	m_bojoInput.m_sOpen = strOpen;		// �ð�
	m_bojoInput.m_sHigh = strHigh;		// ��
	m_bojoInput.m_sLow = strLow;			// ����
	//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen = strPreOpen;		// ���Ͻð�
	m_bojoInput.m_sPreHigh = strPreHigh;		// ���ϰ�
	m_bojoInput.m_sPreLow = strPreLow;		// ��������
	//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice = strPrePrice;	// ��������
//	m_bojoInput.m_sPreVolume = strPreVolume;	// ���ϰŷ���
	m_bojoInput.m_sStartTime = strResetTime;	// �ŷ����۽ð�(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// ��ȸ�� ù���� ����.
	m_bojoInput.m_sFirstClose.TrimRight();
	m_bojoInput.m_sBaseValue = strPrePrice;//strStandard;

	m_bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sNxtFlg = m_InData.Snextgb[0];

	m_bojoInput.m_sTerm.Format("%d", nType);
	m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	if(nType == 1)
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

	m_bojoInput.m_sVolAmt = strVolAmt;	//[A00000651][A00000652]�����ŷ����.
	m_bojoInput.m_szMarketTime	= strMarketTime;	//������.

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	// �ֽļ����� FID ���� ���ɰ� ���������� ����ϳ�,
	// �����޽��� �ɼ��� �ٸ��Ƿ� 134�� FID���� ������� �ʴ� ��ȣ�� ���Ƿ� ����Ͽ���.
	sprintf(szKey, _T("%03d"), 134);
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
