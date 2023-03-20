// ChartItemForeign.cpp: implementation of the CChartItemForeign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemForeign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//@IBK : ��.��.�� ������ �ۿ� ����. ���� �߰��Ǹ� �Ѵ�. 2012
#ifndef LOCALTIME_SET
char* szTickRowFieldForeign[] ={"104302", "104034", "104035", "104023", "104029", "104030", "104031", "104032", };
char* szSecRowFieldForeign[] ={ "104302", "104034", "104035", "104023", "104029", "104030", "104031", "104032", };
char* szMinRowFieldForeign[] = {"104302", "104034", "104035", "104023", "104029", "104030", "104031", "104032", };
char* szDayRowFieldForeign[] = {"52302",					  "52023", "52029", "52030", "52031", "52027", };
#else
char* szTickRowFieldForeign[] ={"104302", "104034", "104023", "104029", "104030", "104031", "104032", };
char* szSecRowFieldForeign[] ={ "104302", "104034", "104023", "104029", "104030", "104031", "104032", };
char* szMinRowFieldForeign[] = {"104302", "104034", "104023", "104029", "104030", "104031", "104032", };
char* szDayRowFieldForeign[] = {"52302",			"52023", "52029", "52030", "52031", "52027", };
#endif

CChartItemForeign::CChartItemForeign()
{
	m_bSet = FALSE;
}

CChartItemForeign::~CChartItemForeign()
{

}

void CChartItemForeign::SetInData(KB_p0606_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0606_InRec1));
	m_bSet = TRUE;
}

CString CChartItemForeign::Convert()
{
	if(!m_bSet) return "";

//	50021
//	104309 // 1: �ѱ��ð�, 0:�����ð�.

	CString strData, strCode, strTemp, strDate;
	int i, nCount;
	
	strTemp = CString(m_InData.num, sizeof(m_InData.num));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Shcode, sizeof(m_InData.Shcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.unit, sizeof(m_InData.unit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.button, sizeof(m_InData.button)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strDate	= CDataConverter::Char2String(m_InData.date, sizeof(m_InData.date));
	int nFlag     = CDataConverter::Char2Int(m_InData.nflag, sizeof(m_InData.nflag));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////

	// ��Ʈ ������
	strData += "51301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sCELL;
	CString stRowData;
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	//NEXTKEY : KHD
	CString stNextData, stNextKey, stNextKey2;
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.nkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	//KHD : ���� ƽ. ��. �� ������ ���� IBK���� ���������� ����. 
	if(nType== 0 || nType == 1 || nType == 6) return "";
	switch(nType)
	{
	case 0:		// ƽ
/*		strData += "104306"; // ��ȸ���� Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;
		
		strData += "104307"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;
*/
/*	KHD	strData += "104308"; // GAP Input �ɺ�
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;
#ifndef LOCALTIME_SET
		strData += "104309";
		strData += sDS;
		strTemp.Format("%c", m_InData.timeGB[0]);
		strData += strTemp;
		strData += sFS;
#endif
*/
		break;

	case 1:		// �� //KHD : ���� ������ ���� ������ ���ֱ�� �ٲ۴�.
	
		break;
/*		strData += "104306"; // ��ȸ���� Input �ɺ�
		strData += sDS;
		strTemp = " ";//CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;

		strData += "104307"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;
*/
/*	KHD	strData += "104308"; // �к��� Input �ɺ�
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

#ifndef LOCALTIME_SET
		strData += "104309";
		strData += sDS;
		strTemp.Format("%c", m_InData.timeGB[0]);
		strData += strTemp;
		strData += sFS;
#endif
*/
		break;

	case 2:		// ��
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?52500");
		stNextKey = "52324";
		stNextData.Format("%8.8s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;
	case 3:		// ��
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?52500");
		stNextKey = "52324";
		stNextData.Format("%8.8s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;
	case 4:		// ��
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?52500");
		stNextKey = "52324";
		stNextData.Format("%8.8s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 

		break;

/*		strData += "104306"; // ��ȸ������ Input �ɺ�  <<!>> ����¥�� ����
		strData += sDS;
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;
*/
/* KHD		strData += "104308"; // ��(1)��(2)��(3)���� Input �ɺ�
		strData += sDS;
		strTemp = (nType==2) ? "1":(nType==3 ? "2":"3");
		strData += strTemp;
		strData += sFS;*/
/*
		// ������ �ʿ������ ����	
		strData += "104309"; // ���ἱ������ Input �ɺ�
		strData += sDS;
		strTemp = " "; //CString(m_InData.modGb, sizeof(m_InData.modGb));
		strData += strTemp;
		strData += sFS;
*/
		break;
	case 6:		// ��
/*KHD
		strData += "101307"; // ���� ���� (1:����, 0:�ƴ�) Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;
		
		strData += "101308"; // n �� Input �ɺ�
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

#ifndef LOCALTIME_SET
		strData += "104309";
		strData += sDS;
		strTemp.Format("%c", m_InData.timeGB[0]);
		strData += strTemp;
		strData += sFS;
#endif
		*/
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound
						// �����ڵ�,  �̸�,  ���簡, ��������,����, �����, �����ŷ���
	char* pFOSymbol[] = { "51301", "51022", "51023", "51313", "51024", "51033", "51027",
						// �ð�,  ��,  ����,// ���Ͻð�,  ���ϰ�,  ��������,
						"51029", "51030", "51031"};// "51029", "51030", "51031", };
						// ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,
//							"30354", "30355", "30358", "30359", "30360", };

	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;
	}

	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	//////////////////////////////////////////////////////////////////////////

	// íƮ �ɺ�


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
		nSize = sizeof(szTickRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldForeign[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = sizeof(szMinRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldForeign[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		nSize = sizeof(szDayRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldForeign[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = sizeof(szSecRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldForeign[i];
			strData += sCOL;
		}
		break;
	}

	strData += sCELL;

	return strData;
}

char* CChartItemForeign::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0606_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit;
	CString strPreHigh, strPreLow, strPreOpen;

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
	double dwRate = atof(strNode);
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // �ŷ���
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	//[CSA00000065] Make sign value
	if(dwRate < 0)			stOut.sign[0] = '5'; // DOWN
	else if(dwRate > 0) stOut.sign[0] = '2'; // UP
	else stOut.sign[0] = '3'; // SAME (BOHAP)

	strOpen = CDataConverter::Parser(strSrc, sCELL); // �ð�
	strHigh = CDataConverter::Parser(strSrc, sCELL); // ��
	strLow = CDataConverter::Parser(strSrc, sCELL); // ����
	strOpen.Remove(_T('.'));
	strHigh.Remove(_T('.'));
	strLow.Remove(_T('.'));
/*
	strPreOpen = CDataConverter::Parser(strSrc, sFS);		// ���Ͻð�
	strPreOpen.Remove(_T('+'));
	strPreHigh = CDataConverter::Parser(strSrc, sFS);		// ���ϰ�
	strPreHigh.Remove(_T('+'));
	strPreLow = CDataConverter::Parser(strSrc, sFS);		// ��������
	strPreLow.Remove(_T('+'));
*/	
	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

	int nSize, nCnt, nType;
	strNode = CString(m_InData.button, sizeof(m_InData.button)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	nType = atoi(strNode);
	if(nType == 0) // ƽ
		nSize = sizeof(szTickRowFieldForeign)/sizeof(char*);
	else if(nType == 1) //��
		nSize = sizeof(szMinRowFieldForeign)/sizeof(char*);
	else if(nType == 6) //��
		nSize = sizeof(szSecRowFieldForeign)/sizeof(char*);
	else		// ��, ��, ��
		nSize = sizeof(szDayRowFieldForeign)/sizeof(char*);
	
	// �׸��� ���
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));

	if (strNextKey == "99999999999999" || atoi(strNextKey) == 0)	// 9�� 14�ڸ��� ����Ű�� ���°�.
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
		//END
	
	CStringArray arrNode;
	int nNodeIndex = 1;
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
		strNode.TrimLeft(); strNode.TrimRight();
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// ��ü ������ �� ���
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// ��Ʈ �� ������
	int nBongSize = sizeof(KB_p0606_OutRec2);
	int nBufSize = sizeof(KB_p0606_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0606_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0606_OutRec2 stFirstBong;

	char cTimeGB = m_InData.timeGB[0];
	int nIndex = 0;
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	int nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);

	KB_p0606_OutRec2 stBong;
	int nRow = 0;
	for( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);

		// time : ƽ,��,���� ��
		if	(nType < 2 || nType == 6)
		{
			strTime = arrNode.GetAt(nIndex++);	// �ѱ��ð�
#ifndef LOCALTIME_SET
			if (cTimeGB == '1')
				strNode = arrNode.GetAt(nIndex++);	// �����ð� ������.
			else
				strTime = arrNode.GetAt(nIndex++);	// �����ð� ���.
#endif
		}
		
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
		//strNode.Format("%d", atoi(strNode));
		//KHD : �ڿ� �������� 2�ڸ��� ���ͼ� ���Ƿ� �����Ѵ�. : �̺κ� IBK���� Ȯ�����ֱ�� ���� ( 2012.2.10)
		if(strNode.GetLength() > 0)
		{
		//	strNode.TrimRight("0");
		//	strNode+="0";
		//	strNode.Delete(strNode.GetLength()-2,2);

		}
		//END 
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

		// ������ ���ۿ� ����, ���ֿ��� ��,��,ƽ�� ������ �ݴ�
	//	if (nType == 6)
	//		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
	//	else
	//		memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);//KHD : �ݴ�� �ͼ� �ݴ�� ���� 
		// ù �����͸� ������ ���´�.
		if ( nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	if (nCnt > 0 && nType == 3 && m_InData.nflag[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

	int nDataType = int(m_InData.dataType[0] - '0'); 
	if(nDataType > 0)
	{
		CString strData(stFirstBong.close, sizeof(stFirstBong.close));
		int nFirstClose = (int)fabs(atof(strData));
		double dDenom,dPrePrice,dOpen; //�и�
		KB_p0602_OutRec2 stBong;
		dPrePrice = fabs(atof(strPrePrice));
		dOpen = fabs(atof(strOpen));
		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;		break;	// ù���� ����
		case 2:	dDenom = 1;					break;	// $  ������ ó������
		case 3:	dDenom = dOpen;			break;	// ���Ͻð�
		case 4:	dDenom = nPrice;			break;	// ���簡
		case 5:	dDenom = dPrePrice;		break;	// ��������
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
//	m_bojoInput.m_sUpLmtPrice = strHighLimit;	// ���Ѱ�
//	m_bojoInput.m_sDnLmtPrice = strLowLimit;	// ���Ѱ�
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
//	m_bojoInput.m_sStartTime = "090000";	// �ŷ����۽ð�(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// ��ȸ�� ù���� ����.

	m_bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sNxtFlg = m_InData.nflag[0];

	if(nType == 0)
	{
		m_bojoInput.m_sTerm = "0";	//	ƽ
		m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
	}
	else if(nType == 1)
	{
		m_bojoInput.m_sTerm = "1";	//	��
		m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
		//{{JS.Kim_20100903 Solomon 60�к� �̻��� ��� �ð����� ���� �����Ͽ��� ��
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
	}
	else if(nType == 6)
	{
		m_bojoInput.m_sTerm = "6";	//	��
		m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
	}
	else if(nType >= 2)
	{
		m_bojoInput.m_sTerm.Format("%d", nType);	//	��,��,��
		m_bojoInput.m_sTick = _T("1");
	}

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 104); // 103�� ����
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
