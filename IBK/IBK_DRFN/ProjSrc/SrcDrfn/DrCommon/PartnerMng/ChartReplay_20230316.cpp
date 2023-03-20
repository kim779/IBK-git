// ChartReplay.cpp: implementation of the CChartReplay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartReplay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szReplayRowField[] = {"302", "034", "023", "032", NULL};

CChartReplay::CChartReplay()
{
	int nIndex=0;
	while (szReplayRowField[nIndex] != NULL)
		nIndex++;

	m_nRepColCnt = nIndex;
}

CChartReplay::~CChartReplay()
{

}

void CChartReplay::SetInData(KB_d1016_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1016_InRec1));
	m_bSet = TRUE;
}

// typedef struct
// {
//     char    shcode[16];                            /* �����ڵ�(16)					 */
//     char    remindDate[2];                         /* �������n����(2)                */
//     char    remindTime[6];                         /* ����ð�(hhmmss)(6)             */
// }KB_d1016_InRec1;

CString CChartReplay::Convert()
{
	if(!m_bSet) return "";
	m_nFutOpt = STOCK;

	CString strData, strCode, strTemp, strFirstTime;
	int i;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));
	strCode.TrimLeft(); strCode.TrimRight();
	
	if (strCode.GetLength() < 6) return "";
	else if (strCode.Left(3) == "101" )//�����϶�..
		m_nFutOpt = FUTURE;
	else if(strCode.Left(3) == "201" || strCode.Left(3) == "301")
		m_nFutOpt = OPTION;
	
	strTemp = CString(m_InData.remindTime, sizeof(m_InData.remindTime));
	strTemp.TrimLeft(); strTemp.TrimRight();
	strFirstTime = strTemp;

	if (strTemp.IsEmpty())		m_nPage = 0;
	else						m_nPage++;

//////////////////////////////////////////////////////////////////////////
//	Inbound ������ �ۼ�
//////////////////////////////////////////////////////////////////////////
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	//	�����ڵ�
	if (m_nFutOpt == FUTURE)	strData += "31301";	
	else if (m_nFutOpt == OPTION)	strData += "41301";		
	else			strData += "1301";
	strData += sDS;
	strData += strCode;
	strData += sCELL;

	if (m_nFutOpt == FUTURE)	strData += "32340"; // ����/���ϱ��� Input �ɺ�
	else if (m_nFutOpt == OPTION)	strData += "42340"; // ����/���ϱ��� Input �ɺ�
	else			strData += "2340"; // ����/���ϱ��� Input �ɺ�
	strData += sDS;
	strTemp = CString(m_InData.remindDate, sizeof(m_InData.remindDate));
	if (atoi(strTemp) == 0)	
		strTemp.Format("%d%s", 0, strFirstTime); // 1 : ����, 0 : ����
	else	
		strTemp.Format("%d%s", 1, strFirstTime);
	strData += strTemp;
	strData += sCELL;

//Data ���� : KHD 
	CString stRowData, stNextData, stNextKey;
	Datah.dindex = GI_TICK;
	Datah.dunit = GI_TICK;
	if(m_nFutOpt == FUTURE)
	{
		stRowData = _T("?32500");
		stNextKey = "32324";
		stNextData.Format("%28.28s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		
		strData += "31301";
		strData += sCELL;
		strData += "31900";
		strData += sCELL;
		strData += "31901";
		strData += sCELL;
	}
	else if(m_nFutOpt == OPTION)
	{
		stRowData = _T("?42500");
		stNextKey = "42324";
		stNextData.Format("%28.28s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		
		strData += "41301";
		strData += sCELL;
		strData += "41900";
		strData += sCELL;
		strData += "41901";
		strData += sCELL;
	}
	else//�ֽ��ϰ�� 
	{
		stRowData = _T("?2500");
		stNextKey = "2324";
		stNextData.Format("%28.28s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		
		strData += "1301";
		strData += sCELL;
		strData += "1900";
		strData += sCELL;
		strData += "1901";
		strData += sCELL;
	}

	strData += stNextKey;
	strData += sCELL;

	//////////////////////////////////////////////////////////////////////////
	// Row Data ����
	strData += stRowData;
	strData += sDS;
	
	CString temp;
	//��ȸ������ �� 
	int GrpHLen = 0;
	temp.Format("%06d", 5000);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = 0 + 0x30;
	//CString strDate	= CDataConverter::Char2String(m_InData.date, sizeof(m_InData.date));
	//CopyMemory(Datah.pday, strDate, strDate.GetLength());
	
	temp.Format("%0*d", sizeof(Datah.lgap), 1);// 1ƽ 
	CopyMemory(Datah.lgap, temp, temp.GetLength() );
	
	char szData_i[sz_DATAH];
	memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
	CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : �ǳ��� 0x00���ڸ� ���� �ִ´�.
	strData += szData_i;
	for (i=0; i<m_nRepColCnt; i++)
	{
		if (m_nFutOpt == FUTURE)	strData += "32";
		else if(m_nFutOpt == OPTION) strData += "42";
		else			strData += "5";

		strData += szReplayRowField[i];
		strData += sCOL;
	}
	strData += sCELL;

	return strData;
}

char* CChartReplay::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_d1016_OutRec1 stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strStartTime, strEndTime;

	memset(&stOut, 0x20, sizeof(stOut));

	memcpy(stOut.remindDate, m_InData.remindDate, sizeof(stOut.remindDate));

	strCode = CDataConverter::Parser(strSrc, sCELL); // �����ڵ�
	strStartTime = CDataConverter::Parser(strSrc, sCELL) + _T("00");
	strEndTime = CDataConverter::Parser(strSrc, sCELL) + _T("00");
	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);
	if (strNextKey !="99999999999999")// 9�� 14�ڸ��� ����Ű�� ���°�.  
	{
		//	CString strKey;
		//	LPTSTR lpszKey = strKey.GetBuffer(sizeof(stOut.nkey) + 1);
		//	::memcpy(lpszKey, strNextKey, sizeof(stOut.nkey));
		//	strNextKey.Format("%s", lpszKey);
		
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
	}
	else
	{
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	}
	//KHD :
	// �׸��� ���
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut));

	strSrc = strSrc.Mid(sizeof(gridOut));
	
 
//	if ((gridOut.xpos[0] & 0x01) || (gridOut.xpos[0] & 0x02))
//		memcpy(stOut.nkey, gridOut.page, sizeof(stOut.nkey));

	//091027-alzioyes.���� DRFN��Ʈ���� �Ҽ������� �����͸� �޾Ҵ�. ����.
	strSrc.Remove('.');

	CStringArray arrNode;
	int nNodeIndex = 1;
	while (strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == m_nRepColCnt)?sROW:sCELL);
		arrNode.Add(strNode);

		if (nNodeIndex == m_nRepColCnt) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// ��ü ������ �� ���
	int nCnt = arrNode.GetSize() / m_nRepColCnt;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// ��Ʈ �� ������
	int nBongSize = sizeof(KB_d1016_OutRec2);
	int nBufSize = sizeof(KB_d1016_OutRec1) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_d1016_OutRec1);

	nDataLen = nBufSize;

	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	memset(pDataBuf, 0x20, nBufSize+1);
	KB_d1016_OutRec2 stFirstBong;

	int nIndex = 0;
	for (int nRow=0; nRow<nCnt; nRow++)
	{
		KB_d1016_OutRec2 stBong;
		memset(&stBong, 0x20, sizeof(stBong));

		// date
		strDate = arrNode.GetAt(nIndex++);
//		OutputDebugString("KHD : " + strDate);
		memcpy(stBong.date, strDate, min(sizeof(stBong.date), strDate.GetLength()));
		// time
		strTime = arrNode.GetAt(nIndex++);
//			OutputDebugString("KHD : " + strTime);
		memcpy(stBong.time, strTime, min(sizeof(stBong.time), strTime.GetLength()));
		// ����/ü�ᰡ
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
		// �ŷ���
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.cvol, strNode, min(sizeof(stBong.cvol), strNode.GetLength()));

		// ������ ���ۿ� ����, ���ֿ��� ��,��,ƽ�� ������ �ݴ�
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		if (nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	char szBojo[400+1];
	memset(szBojo, 0x20, sizeof(szBojo));

	if (atoi(strEndTime) < 153000)
		strEndTime = _T("153000");

	sprintf(szBojo, _T("0400SC=%s@MARKETTIME=%s|%s@"), (LPCSTR)(LPCTSTR)strCode,
						(LPCSTR)(LPCTSTR)strStartTime, (LPCSTR)(LPCTSTR)strEndTime);
	memcpy(stOut.bojo, szBojo, min(strlen(szBojo), sizeof(stOut.bojo)));

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}
