// ChartItemFOForeign.cpp: implementation of the CChartItemFOForeign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFOForeign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowFieldFOFo[] ={"111302", "111034", "111023", "111029", "111030", "111031", "111032", "111028", NULL};
char* szSecRowFieldFOFo[] =	{"111302", "111034", "111023", "111029", "111030", "111031", "111032", "111028", NULL};
char* szMinRowFieldFOFo[] =	{"111302", "111034", "111023", "111029", "111030", "111031", "111032", "111028", NULL};
char* szDayRowFieldFOFo[] =	{"111302",           "111023", "111029", "111030", "111031", "111027", "111028", NULL};

CChartItemFOForeign::CChartItemFOForeign()
{
	m_bSet = FALSE;
	m_nTickCnt = 0;	m_nSecCnt = 0;	m_nMinCnt = 0;	m_nDayCnt = 0;
	while (szTickRowFieldFOFo[m_nTickCnt] != NULL)		m_nTickCnt++;
	while (szSecRowFieldFOFo[m_nSecCnt] != NULL)		m_nSecCnt++;
	while (szMinRowFieldFOFo[m_nMinCnt] != NULL)		m_nMinCnt++;
	while (szDayRowFieldFOFo[m_nDayCnt] != NULL)		m_nDayCnt++;
}

CChartItemFOForeign::~CChartItemFOForeign()
{

}

void CChartItemFOForeign::SetInData(HCQ03010_IN* pData)
{
	memcpy(&m_InData, pData, sizeof(HCQ03010_IN));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
//int gCntI=0;
CString CChartItemFOForeign::Convert()
{
	if(!m_bSet) return "";

	CString strData, strCode, strTemp, strImgRgb;
	int i, nCount;

	strTemp = CString(m_InData.dcnt, sizeof(m_InData.dcnt));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.jmcode, sizeof(m_InData.jmcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.nsel, sizeof(m_InData.nsel));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.dgubun, sizeof(m_InData.dgubun)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strImgRgb = _T("1");//CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));

	//	Inbound		//////////////////////////////////////////////////////////

	// ��Ʈ ������
	strData += "70301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sFS;

	switch(nType)
	{
	case 0:		// ƽ
		// ��ȸ���� Input �ɺ�
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += "111306";	strData += sDS;	strData += strTemp;	strData += sFS;

		// ���ϸ���ȸ Input �ɺ�
//		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strTemp = _T("0");
		strData += "111307";	strData += sDS;	strData += strTemp;	strData += sFS;

		// GAP Input �ɺ�
		strTemp.Format("%d", nUnit);
		strData += "111308";	strData += sDS;	strData += strTemp;	strData += sFS;
/*
		if (m_InData.Srecordgb[0] == '1')
		{
			strData += "111304";		//	������Ʈ ��ȸ �ð�
			strData += sDS;
			strTemp = CString(m_InData.chetime, sizeof(m_InData.chetime));
			strData += strTemp;
			strData += sFS;
		}
*/
		break;

	case 1:		// ��
		// ��/��� ���� (0:���, 1:�Ǻ�)
		//strData += "111305";	strData += sDS;	strData += strImgRgb;	strData += sFS;

		strData += "111306"; // ��ȸ���� Input �ɺ�
		strData += sDS;
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;

		strData += "111307"; // ���ϸ���ȸ Input �ɺ�
		strData += sDS;
		//		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strTemp = _T("0");
		strData += strTemp;
		strData += sFS;

		strData += "111308"; // �к��� Input �ɺ�
		strData += sDS;
		strTemp.Format("%03d", nUnit);
		strData += strTemp;
		strData += sFS;
/*
		if (m_InData.Srecordgb[0] == '1')
		{
			strData += "111304";		//	������Ʈ ��ȸ �ð�
			strData += sDS;
			strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			strData += strTemp;
			strData += sFS;
		}
*/
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
// 		strData += "101305"; // ��ȸ�������� Input �ɺ�
// 		strData += sDS;
// 		strTemp = CString(m_InData.date, sizeof(m_InData.date));
// 		strData += strTemp;
// 		strData += sFS;

		strData += "111306"; // ��ȸ������ Input �ɺ�  <<!>> ����¥�� ����
		strData += sDS;
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;
		
		strData += "111308"; // ��(1)��(2)��(3)���� Input �ɺ�
		strData += sDS;
		strTemp = (nType==2) ? "001":(nType==3 ? "002":"003");
		strData += strTemp;
		strData += sFS;

		// ������ �ʿ������ ����	
		/*
		strData += "111309"; // ���ἱ������ Input �ɺ�
		strData += sDS;
		strTemp = " "; //CString(m_InData.modGb, sizeof(m_InData.modGb));
		strData += strTemp;
		strData += sFS;
		*/
		break;
	case 6:		// ��
		// ��/��� ���� (0:���, 1:�Ǻ�)
		//strData += "111305";	strData += sDS;	strData += strImgRgb;	strData += sFS;

		strData += "111307"; // ���� ���� (1:����, 0:�ƴ�) Input �ɺ�
		strData += sDS;
//		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strTemp = _T("0");
		strData += strTemp;
		strData += sFS;
		
		strData += "111308"; // n �� Input �ɺ�
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

		break;
	}

	//	OutBound	//////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� 
	strData += "70301";	strData += sFS;	strData += "70309";	strData += sFS;	// �����ڵ�,	����ǥ�ñ���

	// ��������(70356) ��ſ� ����(60028) ��ȸ.
	//strData += "71023";	strData += sFS;	strData += "70356";	strData += sFS;	// ���簡,		��������
	strData += "71023";	strData += sFS;	strData += "70320";	strData += sFS;	// ���簡,		����

	strData += "71024";	strData += sFS;	strData += "71033";	strData += sFS;	// ����,		�����
	//strData += "71027";	strData += sFS;	strData += "70354";	strData += sFS;	// �����ŷ���,	���Ѱ�
	strData += "71027";	strData += sFS;	strData += "70354";	strData += sFS;	// �����ŷ���,	���Ѱ�
	strData += "70355";	strData += sFS;	strData += "71029";	strData += sFS;	// ���Ѱ�,		�ð�
	strData += "71030";	strData += sFS;	strData += "71031";	strData += sFS;	// ��,		����
	strData += "70358";	strData += sFS;	strData += "70359";	strData += sFS;	// ���Ͻð�,	���ϰ�
	strData += "70360";	strData += sFS;	strData += "70022";	strData += sFS;	// ��������,	�̸�
	strData += "70325";	strData += sFS;	strData += "70326";	strData += sFS;	// ����۽ð�,	������ð�(KST)
	strData += "70310";	strData += sFS;	strData += "70305";	strData += sFS;	// ����۽ð�,	������ð�(KST)

	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound

	// íƮ �ɺ�
	strData += _T('$');
	if(nType == 0) strData += "111410";
	else if(nType == 1) strData += "111510";
	else if(nType == 6) strData += "111511";
	else strData += "111610";
	strData += sDS;

	// �׸��� ���
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));


	char szTemp[5];

	memcpy(grid_i.vrow, _T("11"), sizeof(grid_i.vrow));

	sprintf(szTemp, _T("%04d"), nCount);	// ��ȸ ������ ��
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));

	grid_i.vflg[0] = _T('1');
	grid_i.gdir[0] = _T('1');	//	top.
	grid_i.sdir[0] = _T('1');

	if (m_InData.gubun[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	�Ϲ�(Next��ȸ).

	memcpy(grid_i.page, m_InData.keyvalue, 4);
	memcpy(grid_i.save, m_InData.keyvalue+4, sizeof(grid_i.save));

/*
	grid_i.gdir[0] = _T('1');	//	top.
	if (m_InData.gubun[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	�Ϲ�(Next��ȸ).


	memcpy(grid_i.page, m_InData.keyvalue, 4);
	memcpy(grid_i.save, m_InData.keyvalue+4, sizeof(grid_i.save));

	grid_i.sdir[0] = _T('1');
	grid_i.vflg[0] = _T('1');
	memcpy(grid_i.vrow, _T("999"), sizeof(grid_i.vrow));
*/
	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strData += szGrid_i;

	// Row Data ����
	int nSize;
	switch(nType)
	{
	case 0:		// ƽ
		nSize = m_nTickCnt;
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldFOFo[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = m_nMinCnt;
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldFOFo[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		nSize = m_nDayCnt;
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldFOFo[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = m_nSecCnt;
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldFOFo[i];
			strData += sCOL;
		}
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	
	strData += sCELL;

// 	{
// 		CString szLog;
// 		szLog.Format("c:\\Logs\\FI%05d.txt", gCntI++);
// 		FILE* fp = fopen(szLog, "w");
// 		if(fp)
// 		{
// 			fwrite(strData, 1, strData.GetLength(), fp);
// 			fclose(fp);
// 		}
// 	}

	return strData;
}

//int gCnt = 0;
char* CChartItemFOForeign::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
// 	{
// 		CString szLog;
// 		szLog.Format("c:\\Logs\\FO%05d.txt", gCnt++);
// 		FILE* fp = fopen(szLog, "w");
// 		if(fp)
// 		{
// 			fwrite(pData, 1, nLen, fp);
// 			fclose(fp);
// 		}
// 	}

	EU_p0602_OutRec1 stOut1;
	EU_p0602_CHART stOut;
	int nIndex = 0;
	char *pszBuff = NULL, *pszData=NULL;
	pszBuff = new char[nLen+1];
	memset(pszBuff, 0x00, nLen+1);
	memcpy(pszBuff, pData, nLen);

	pszData = pszBuff;
	
	CString strNode, strName, strLock, strDecimalGB, strLastDate;
	CString strPrice, strVolume, strSDate, strEDate, strMType, strMDay;

	double dOpen, dHigh, dLow, dPreOpen, dPreHigh, dPreLow, dMaxHigh, dMinLow;
	double dPrePrice, dChRate=0.0;
	
	dPrePrice = dOpen = dHigh = dLow = dPreOpen = dPreHigh = dPreLow = dMaxHigh = dMinLow = 0;

	memset(&stOut, 0x20, sizeof(stOut));
	memset(&stOut1, 0x20, sizeof(stOut1));

	int nPrice = 0;

	pszData += CDataConverter::Parser2(pszData, sFS, m_bojoInput.m_sShcode) + 1;	// �����ڵ�
	pszData += CDataConverter::Parser2(pszData, sFS, strDecimalGB) + 1;	// ��������(����ǥ�ñ���)

	if(m_bojoInput.m_sShcode.IsEmpty())
	{
		nDataLen = 0;

		delete []pszBuff;
		pszBuff = NULL;
		
		return NULL;
	}

	double dValue=0, dRate;
	int nRow, nPcnt;
	CString strRes;
	char cType = CDataConverter::GetDecimalRate(strDecimalGB, nPcnt, dRate, strRes);
	stOut1.codetype[0] = cType;

	// ���簡
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	strNode = strNode.Mid(1);
	m_bojoInput.m_sPrice = strNode;
	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	memcpy(stOut1.price, strNode, min(sizeof(stOut1.price), strNode.GetLength()));
	CString szCur = strNode;

	// �������� -> ����(���ذ�)
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	//if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
		//strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	memcpy(stOut1.preprice, strNode, min(sizeof(stOut1.preprice), strNode.GetLength()));
	m_bojoInput.m_sPreprice = strNode;

	// ��������
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	memcpy(stOut1.sign, strNode.Left(1), 1);
	strNode = strNode.Mid(1);
	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	memcpy(stOut1.change, strNode, min(sizeof(stOut1.change), strNode.GetLength()));

	// �����
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	dChRate = atof(strNode);//	strNode.Remove(_T('+'));strNode.Remove(_T('-'));strNode.Remove(_T('.'));
	memcpy(stOut1.chrate, strNode, min(sizeof(stOut1.chrate), strNode.GetLength()));

	pszData += CDataConverter::Parser2(pszData, sFS, strVolume) + 1;	// �ŷ���
	memcpy(stOut1.volume, strVolume, min(sizeof(stOut1.volume), strVolume.GetLength()));
	m_bojoInput.m_sVolume = strVolume;

//Solomon100613:alzioyes �ؿ����񿡼��� �����Ѱ� ����. -->
	// ���Ѱ�
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
// 	//	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
// 	//		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
// 		m_bojoInput.m_sUpLmtPrice = strNode;

	// ���Ѱ�
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
// 	//	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
// 	//		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
// 		m_bojoInput.m_sDnLmtPrice = strNode;
//<--

	// �ð�
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	strNode = strNode.Mid(1);
	m_bojoInput.m_sOpen = strNode;
	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	memcpy(stOut1.open, strNode, min(sizeof(stOut1.open), strNode.GetLength()));

	// ��
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	strNode = strNode.Mid(1);
	m_bojoInput.m_sHigh = strNode;
	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	memcpy(stOut1.high, strNode, min(sizeof(stOut1.high), strNode.GetLength()));

	// ����
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
	strNode = strNode.Mid(1);
	m_bojoInput.m_sLow = strNode;
	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	memcpy(stOut1.low, strNode, min(sizeof(stOut1.low), strNode.GetLength()));

	// ���Ͻð�
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
//	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
//		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	m_bojoInput.m_sPreOpen = strNode;

	// ���ϰ�
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
//	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
//		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	m_bojoInput.m_sPreHigh = strNode;

	// ��������
	pszData += CDataConverter::Parser2(pszData, sFS, strNode) + 1;
//	if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
//		strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
	m_bojoInput.m_sPreLow = strNode;

	pszData += CDataConverter::Parser2(pszData, sFS, strName) + 1;					// �����
	memcpy(stOut1.name, strName, min(strName.GetLength(), sizeof(stOut1.name)));

	pszData += CDataConverter::Parser2(pszData, sFS, strSDate) + 1;	// ����۽ð�(KST)
	pszData += CDataConverter::Parser2(pszData, sFS, strEDate) + 1;	// ������ð�(KST)
	pszData += CDataConverter::Parser2(pszData, sFS, strMType) + 1;	// �屸��
	pszData += CDataConverter::Parser2(pszData, sFS, strMDay) + 1;	// �������

	int nSize, nCnt, nType;
	strNode = CString(m_InData.dgubun, sizeof(m_InData.dgubun)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	nType = atoi(strNode);
	if (nType == 0)			nSize = m_nTickCnt;
	else if(nType == 1)		nSize = m_nMinCnt;
	else if(nType == 6)		nSize = m_nSecCnt;
	else					nSize = m_nDayCnt;

	// �׸��� ���
	GRAPH_IO gridOut;
	nIndex = sizeof(gridOut);
	memcpy(&gridOut, pszData, nIndex);
	pszData += nIndex;

	//{{Js.Kim_20100906 ����Ÿ�߰��θ���
//	if ((gridOut.xpos[0] & 0x01) || (gridOut.xpos[0] & 0x02))
//		memcpy(stOut1.nkey, gridOut.page, sizeof(stOut1.nkey));
	//}}

	//091027-alzioyes.���� DRFN��Ʈ���� �Ҽ������� �����͸� �޾Ҵ�. ����.
	//	strSrc.Remove('.');

	CStringArray arrNode;
	int nNodeIndex = 1;
	while ((pszData - pszBuff) < nLen-2)
	{
		pszData += CDataConverter::Parser2(pszData, (nNodeIndex == nSize)?sROW:sCELL, strNode) + 1;
		arrNode.Add(strNode);
		
		if (nNodeIndex == nSize)
			nNodeIndex = 1;
		else							nNodeIndex++;
	}
	
	// ��ü ������ �� ���
	nCnt = arrNode.GetSize() / nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));
	
	// ��Ʈ �� ������
	int nBongSize = sizeof(EU_p0602_OutRec2);
	int	nPosBong = sizeof(EU_p0602_OutRec1) + sizeof(EU_p0602_CHART) - nBongSize;
	int nBufSize = nPosBong + nBongSize * nCnt;
	
	nDataLen = nBufSize;
	
	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	ZeroMemory(pDataBuf, nBufSize); pDataBuf[nBufSize]=0;
	EU_p0602_OutRec2 stFirstBong, stBong;

	nIndex = 0;
	for(nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);

		// time : ƽ,��,���� ��
		if(nType < 2 || nType == 6) 
			strTime = arrNode.GetAt(nIndex++); 
		else
			strTime = _T("000000");

		// 2011.01.13 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		// time : ƽ,��,���� ��
//		if(nType < 2 || nType == 6) 
//			strTime = arrNode.GetAt(nIndex++); 
// 		if(nType == 0) // DDHHMMSS
// 			strNode = strDate.Right(2) + strTime;
// 		else if(nType == 1) //MMDDHHMM
// 			strNode = strDate.Right(4) + strTime.Left(4);
// 		else if(nType == 4) //YYYYMM
// 			strNode = strDate.Left(6);
// 		else if(nType == 6) //DDHHMMSS
// 			strNode = strDate.Right(2) + strTime.Left(6);
// 		else 
// 			strNode = strDate;

		if(nType == 0)		// ƽ : DDHHMMSS
		{
			strNode = strDate + strTime.Left(6);
		}
		else if(nType == 1) // �� : MMDDHHMM ==> YYYYMMDDHHMMSS �� ����
		{
			strNode = strDate + strTime.Left(6);
		}
		else if(nType == 4) // �� : YYYYMM
		{
			strNode = strDate.Left(6);
			strNode += _T("        ");
		}
		else if(nType == 6) // �� : DDHHMMSS
		{
			strNode = strDate + strTime.Left(6);
		}
		else 
		{
			strNode = strDate;
			strNode += _T("      ");
		}

		//strNode.Format("%8s%6s", strDate, strTime);
		// 2011.01.13 by SYS <<

//		if ((nType == 0 && nRow == 0) || (nType != 0 && nRow == nCnt-1))
		if( nRow == nCnt-1 )
		{
			memcpy(stOut1.endDate, strDate, min(strDate.GetLength(), sizeof(stOut1.endDate)));
			//strLastDate = strDate+strTime.Left(6);
		}
		//else if ((nType != 0 && nRow == 0) || (nType == 0 && nRow == nCnt-1))
		if( nRow == 0 )
		{
			memcpy(stOut1.startDate, strDate, min(strDate.GetLength(), sizeof(stOut1.startDate)));
			strLastDate = strDate+strTime.Left(6);
		}
		
		memcpy(stBong.date, strNode, min(sizeof(stBong.date), strNode.GetLength()));
		
		// ����/ü�ᰡ
		strNode = arrNode.GetAt(nIndex++);
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
			strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
		memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
		// �ð�
		strNode = arrNode.GetAt(nIndex++);
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
			strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
		memcpy(stBong.open, strNode, min(sizeof(stBong.open), strNode.GetLength()));
		// ��
		strNode = arrNode.GetAt(nIndex++);
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
			strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
		memcpy(stBong.high, strNode, min(sizeof(stBong.high), strNode.GetLength()));
		// ����
		strNode = arrNode.GetAt(nIndex++);
		if (!CDataConverter::TbondPrcLToF(cType, nPcnt, dRate, strNode))
			strNode.Format(_T("%lf"), CDataConverter::DataSeparation(strNode, nPcnt));
		memcpy(stBong.low, strNode, min(sizeof(stBong.low), strNode.GetLength()));
		// �ŷ���
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.volume, strNode, min(sizeof(stBong.volume), strNode.GetLength()));

		//@Solomon	091202SM123	-->
		// �ŷ����
		strNode = arrNode.GetAt(nIndex++);
		//dValue = atof(strNode)*1000000;
		//strNode.Format(_T("%.0f"), dValue);
		strNode += "000000";
// 		if(nRow==0)
// 		{
// 			CString szDebug;
// 			szDebug.Format("[SMalzio] FOForeign�ŷ����=%s", strNode);
// 			OutputDebugString(szDebug);
// 		}
//		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--

//		if (nType == 0)
//			memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
//		else
			memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);
		
		// ù �����͸� ������ ���´�.
		if ((nType == 0 && nRow == 0) || (nType != 0 && nRow == nCnt-1))
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}
	
	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	if (nCnt > 0 && nType == 3 && m_InData.gubun[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

	//////////////////////////////////////////////////////////////////////////
	// ��Ʈ���� �޽��� �����
	CChartBozoManager bojoMng;
	//	m_bojoInput.m_sPreVolume = strPreVolume;	// ���ϰŷ���
	//	m_bojoInput.m_sStartTime = "090000";	// �ŷ����۽ð�(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// ��ȸ�� ù���� ����.
	
	m_bojoInput.m_sDataType = _T("0");		//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sMarket = _T("1");			//	����. ���� �ʿ�� ����.
	m_bojoInput.m_sNxtFlg = m_InData.gubun[0];
	
	if(nType == 0)
	{
		m_bojoInput.m_sTerm = "0";	//	ƽ
		m_bojoInput.m_sTick = CString(m_InData.nsel, sizeof(m_InData.nsel));
	}
	else if(nType == 1)
	{
		m_bojoInput.m_sTerm = "1";	//	��
		m_bojoInput.m_sTick = CString(m_InData.nsel, sizeof(m_InData.nsel));
		//{{JS.Kim_20100903 Solomon 60�к� �̻��� ��� �ð����� ���� �����Ͽ��� ��
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
	}
	else if(nType == 6)
	{
		m_bojoInput.m_sTerm = "6";	//	��
		m_bojoInput.m_sTick = CString(m_InData.nsel, sizeof(m_InData.nsel));
	}
	else if(nType >= 2)
	{
		m_bojoInput.m_sTerm.Format("%d", nType);	//	��,��,��
		m_bojoInput.m_sTick = _T("1");
	}
	
	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 111); // 111�� �ؿܼ���

	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode, nPcnt);

	CString strTimeIdx, strRate;
	strTimeIdx.Format("%s", CDataConverter::GetTimeIndexInfo(22, 1));	//@Solomon	����ð���� ������ ��� �´�.
	CString strBozoTail;
	strRate.Format(_T("%.*f"), nPcnt, dRate);
	strSDate = strSDate + "00|" + strEDate + "00";
	strName = m_bojoInput.m_sShcode + ":" + strName;

	List_CChartBozoItem arr;
	CChartBozoItem *pItem;

	//100618-alzioyes : �״� �������.
	strLastDate.TrimLeft();	strLastDate.TrimRight();

	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("TIMEDIFF"), strTimeIdx);	arr.AddTail(pItem);
	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETTIME"), strSDate);	arr.AddTail(pItem);
	//{{JS.Kim_20100903 Solomon	�������� ����ð� ���� �����Ƿ� ������ �������� ����
	//pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("DATATIME"), strLastDate);	arr.AddTail(pItem);
	//}}
	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("CODEANDNAME"), strName);	arr.AddTail(pItem);
	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETTYPE"), strMType);	arr.AddTail(pItem);
	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETDAY"), strMDay);		arr.AddTail(pItem);
	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("HBPRICE"), strRate);		arr.AddTail(pItem);
	pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("DECTYPE"), strRes);		arr.AddTail(pItem);
	if (cType >= 'A' && cType <= 'K')
	{
		CString strTDC;
		strTDC.Format(_T("%f:_Open_,|%f:_High_,|%f:_Low_,|%f:_Close_,"), 1.0, 1.0, 1.0, 1.0);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("TDC"), strTDC);				arr.AddTail(pItem);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("RDC"), strTDC);				arr.AddTail(pItem);
	}

	nSize = bojoMng.ChangeChartBozoMsg(strNode, &arr);

	memcpy(stOut.bojomsg, (LPSTR)(LPCTSTR)strNode, nSize);		//	�����޽���
	memcpy(stOut.bojomsg, "0596", 4);

	POSITION pos = arr.GetHeadPosition();
	while (pos)
	{
		pItem = arr.GetNext(pos);
		if (pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}
	arr.RemoveAll();


//	������ ���� �ð� : 2010�� 01�� 21�� 10:24
//"UM=%s@UD=1@RDATET=YYYYMMDD@TDC=%s:�ð�,|%s:��,|%s:����,|%s:����,|1:�⺻�ŷ���,@RDC=%s:%d:�ð�,|%s:%d:��,|%s:%d:����,|%s:%d:����,|1:�⺻�ŷ���,@REALTIMET=YYYYMMDDHHMMSS@TIMEDIFF=%s@PREVPRICE=����:%s|�⺻�ŷ���:%s@MARKETTIME=%s|%s@DATATIME=%s@CODEANDNAME=%s:%s@MARKETTYPE=%s@MARKETDAY=%s@RESETUSEREAL=%d@HBPRICE=%s@",
//UM=E@UD=1@RDATET=YYYYMMDD@TDC=0.00001:�ð�,|0.00001:��,|0.0001:����,|0.00001:����,|1:�⺻�ŷ���,@RDC=0.00001:-1:�ð�,|0.00001:-1:��,|0.00001:-1:����,|0.00001:-1:����,|1:�⺻�ŷ���,@REALTIMET=YYYYMMDDHHMMSS@TIMEDIFF=-6@PREVPRICE=����:0.874500|�⺻�ŷ���:        29@MARKETTIME=1700|1600@DATATIME=20100121191312@CODEANDNAME=LEG10:Live Cattle (2010.2)@MARKETTYPE=1@MARKETDAY=20100121@RESETUSEREAL=1@HBPRICE=0.00010@
//	strBozoTail.Format(_T("REALTIMET=YYYYMMDDHHMMSS@TIMEDIFF=%s@MARKETTIME=%s|%s@DATATIME=%s@CODEANDNAME=%s:%s@MARKETTYPE=%s@MARKETDAY=%s@RESETUSEREAL=%d@HBPRICE=%s@"),
//						strTimeIdx, strSDate, strEDate, strLastDate, m_bojoInput.m_sShcode, strName, strMType, strMDay, 1, strRate);
	////////////////////////////////////////////////////////////////////////////////

	memcpy(pDataBuf, &stOut1, sizeof(stOut1));
	int nOut1Size = sizeof(stOut1);
	memcpy(&pDataBuf[nOut1Size], &stOut, sizeof(stOut) - nBongSize);
	
	delete []pszBuff;
	pszBuff = NULL;
	
	return pDataBuf;
}
