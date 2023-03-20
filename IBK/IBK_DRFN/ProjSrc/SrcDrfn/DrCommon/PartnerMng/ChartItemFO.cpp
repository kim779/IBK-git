// ChartItemFO.cpp: implementation of the CChartItemFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//���� 
char* szTickRowFieldFO[] =	{"32302", "32034", "32023", "32029", "32030", "32031", "32032", "32028", "32201",};
char* szSecRowFieldFO[] =	{"36302", "36034", "36023", "36029", "36030", "36031", "36032", "36028", "36201", };
char* szMinRowFieldFO[] =	{"34302", "34034", "34023", "34029", "34030", "34031", "34032", "34028", "34201",};
char* szDayRowFieldFO[] =	{"35302",           "35023", "35029", "35030", "35031", "35027", "35028", "35201",}; // "35201" �̰��� ���� 
//�ɼ�
char* szTickRowFieldOption[] =	{"42302", "42034", "42023", "42029", "42030", "42031", "42032", "42028", "42201",};
char* szSecRowFieldOption[] =	{"46302", "46034", "46023", "46029", "46030", "46031", "46032", "46028", "46201",};
char* szMinRowFieldOption[] =	{"44302", "44034", "44023", "44029", "44030", "44031", "44032", "45028", "45201",};
char* szDayRowFieldOption[] =	{"45302",   "45023", "45029", "45030", "45031", "45027", "45028", "45201",};

//��ǰ���� 
char* szTickRowFieldCommodity[] =	{"102302", "102034", "102023", "102029", "102030", "102031", "102032", "105028","102201",};
char* szSecRowFieldCommodity[] =	{"106302", "106034", "106023", "106029", "106030", "106031", "106032", "105028", "106201",};
char* szMinRowFieldCommodity[] =	{"104302", "104034", "104023", "104029", "104030", "104031", "104032", "105028", "104201",};
char* szDayRowFieldCommodity[] =	{"105302",   "105023", "105029", "105030", "105031", "105027", "105028","105201", };

CChartItemFO::CChartItemFO()
{
	m_bSet = FALSE;
}

CChartItemFO::~CChartItemFO()
{

}

void CChartItemFO::SetInData(KB_p0604_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0604_InRec1));
	m_bSet = TRUE;
}

// MUltiChart������ ��ȸ ����ü�� �޾Ƽ� FID ���·� ��ȯ
// ���� : ��Ʈ��ȸGrid Inbound + �ü� FID Inbound + GridHeader + Grid Outbound
// winix �÷������� �迭���·� �������� �����͸� Grid ��� �θ���.
CString CChartItemFO::Convert()
{
	CString strCode;
	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	if(!m_bSet) return "";

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	m_nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);

	switch(m_nCodeType)
	{
		case CDRCodeTypeInfo::CODE_KFUTURE://���ἱ��?? 
			return ConvertFuture();
			break;
		case CDRCodeTypeInfo::CODE_FUOPT_COMMODITY://��ǰ���� 
			return ConvertCommodity();	
			break;
		case	CDRCodeTypeInfo::CODE_KOPTION:
			return ConvertOption();//�ɼ��ϰ�� ù�ڸ��� 2,3
			break;
		case CDRCodeTypeInfo::CODE_FUTURE_SPREAD:
			return ConvertFuture();
			break;
	}

	
	return "";
}

/**
	@History
		2011.01.04 alzioyes: ��ǰ�����߿��� ��(), �̴ϱ�(), ����()�� ��쿡�� x1 ���·�, �׿ܴ� x0.01���·� ó���ؾ��ؼ� ������.
*/
CString CChartItemFO::ConvertCommodity()
{
	CString strData, strCode, strTemp, strImgRgb, strDate, strNow;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);
	if(nUnit > 372)
		nUnit = 372;
	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	strDate = CString(m_InData.Sdate, sizeof(m_InData.Sdate));//KHD
	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////

	// ��Ʈ ������

	strData += "101301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	CString stRowData;
	switch(nType)
	{
	case 0:		// ƽ
		Datah.dindex = GI_TICK;
		stRowData = _T("?102500");
		// 		// ���ϸ���ȸ Input �ɺ� (1:����) 
 		strData += "102309";	strData += sDS;	strData += strNow;	strData += sCELL;
		//KHD : NEXT		
		stNextKey = "102324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		break;

	case 1:		// ��
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?104500");
		//KHD : NEXT		
		stNextKey = "104324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// ��/��� ���� (0:���, 1:�Ǻ�)
 		strData += "104308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "104309";//���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strData += strNow;
 		strData += sCELL;

		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "104339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;

	case 2:		// ��
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?105500");
		//KHD : NEXT		
		stNextKey = "105324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "105339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 3:		// ��
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?105500");
		//KHD : NEXT		
		stNextKey = "105324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "105339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 4:		// ��
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?105500");
		//KHD : NEXT		
		stNextKey = "105324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "105339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 6:		// ��
		Datah.dindex = GI_SEC;
		stRowData = _T("?106500");
		//KHD : NEXT		
		stNextKey = "106324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// ��/��� ���� (0:���, 1:�Ǻ�)
 		strData += "106308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "106309";//���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strData += strNow;
 		strData += sCELL;

		// 20130530 �̻�� : ������ ������ �߰�(IBK) (Issue No : 0006572)
		strData += "106339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound : KHD : IBK ���߱� 
						// �����ڵ�,  �̸�,  ���簡,    ��������  ,����?,     �����, �����ŷ���
	char* pFOSymbol[] = { "101301", "101022", "101023", "101336", "101024", "101033", "101027",
						// ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,    	// ���Ͻð�, ���ϰ�, ��������, �����ŷ����
						"101311", "101312", "101029", "101030", "101031","101320", "101321", "101322", "101315", "101201", "101307", "101900", "101901"};
					
	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;//sFS;
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
		nSize = sizeof(szTickRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldCommodity[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = sizeof(szMinRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldCommodity[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		nSize = sizeof(szDayRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldCommodity[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = sizeof(szSecRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldCommodity[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}
CString CChartItemFO::ConvertFuture()
{
	CString strData, strCode, strTemp, strImgRgb, strDate, strNow;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);
	if(nUnit > 372)
		nUnit = 372;
	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	strDate = CString(m_InData.Sdate, sizeof(m_InData.Sdate));//KHD
	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////

	// ��Ʈ ������

	strData += "31301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	CString stRowData;
	switch(nType)
	{
	case 0:		// ƽ
		Datah.dindex = GI_TICK;
		stRowData = _T("?32500");
		strData += "32309";	strData += sDS;	strData += strNow;	strData += sCELL;
		//KHD : NEXT		
		stNextKey = "32324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;

			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
			strData += "32341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		//END
		break;

	case 1:		// ��
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?34500");
		//KHD : NEXT		
		stNextKey = "34324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// ��/��� ���� (0:���, 1:�Ǻ�)
 		strData += "34308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "34309";//���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strData += strNow;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
			strData += "34341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;

	case 2:		// ��
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?35500");
		//KHD : NEXT		
		stNextKey = "35324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		break;
	case 3:		// ��
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?35500");
		//KHD : NEXT		
		stNextKey = "35324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		break;
	case 4:		// ��
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?35500");
		//KHD : NEXT		
		stNextKey = "35324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		break;
	case 6:		// ��
		// ��/��� ���� (0:���, 1:�Ǻ�)
		Datah.dindex = GI_SEC;
		stRowData = _T("?36500");
		//KHD : NEXT		
		stNextKey = "36324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// ��/��� ���� (0:���, 1:�Ǻ�)
 		strData += "36308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "36309";//���ϸ���ȸ Input �ɺ�
		strData += sDS;
		strData += strNow;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
			strData += "36341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound : KHD : IBK ���߱� 
						// �����ڵ�,  �̸�,  ���簡, ��������,����, �����, �����ŷ���
	char* pFOSymbol[] = { "31301", "31022", "31023", "31336", "31024", "31033", "31027",
						// ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,    	// ���Ͻð�, ���ϰ�, ��������, �����ŷ����
						"31311", "31312", "31029", "31030", "31031","31320", "31321", "31322", "35028", "31201", "31307", "31900", "31901" };
					
	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;//sFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	//////////////////////////////////////////////////////////////////////////

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
		nSize = sizeof(szTickRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldFO[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = sizeof(szMinRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldFO[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		nSize = sizeof(szDayRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldFO[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = sizeof(szSecRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldFO[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}

CString CChartItemFO::ConvertOption()
{
	CString strData, strCode, strTemp, strImgRgb, strDate;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);
	if(nUnit > 372)
		nUnit = 372;

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // ƽ(0)/��(1)/��(2)/��(3)/��(4)
	int nType = atoi(strTemp);

	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	strDate = CString(m_InData.Sdate, sizeof(m_InData.Sdate));//KHD
	CString strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound ������ �ۼ�
	//////////////////////////////////////////////////////////////////////////

	// ��Ʈ ������

	strData += "41301"; // �����ڵ� Input �ɺ�
	strData += sDS;

	strData += strCode;
	strData += sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	CString stRowData;
	switch(nType)
	{
	case 0:		// ƽ
		Datah.dindex = GI_TICK;
		stRowData = _T("?42500");

		strData += "42309";	strData += sDS;	strData += strNow;	strData += sCELL;//����
		//KHD : NEXT		
		stNextKey = "42324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
			strData += "42341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;

	case 1:		// ��
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?44500");
		//KHD : NEXT		
		stNextKey = "44324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// ��/��� ���� (0:���, 1:�Ǻ�)
  		strData += "44308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
 		strData += "44309"; // ���ϸ���ȸ Input �ɺ�
 		strData += sDS;
 		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
 		strData += strTemp;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
			strData += "44341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;

	case 2:		// ��
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?45500");
		//KHD : NEXT		
		stNextKey = "45324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		break;
	case 3:		// ��
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?45500");
		//KHD : NEXT		
		stNextKey = "45324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END

		break;
	case 4:		// ��
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?45500");
		//KHD : NEXT		
		stNextKey = "45324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END

		break;
	case 6:		// ��
		// ��/��� ���� (0:���, 1:�Ǻ�)
		Datah.dindex = GI_SEC;
		stRowData = _T("?46500");
		//KHD : NEXT		
		stNextKey = "46324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // �ؽ�Ʈ Ű�� : KHD 
		//END
		// ��/��� ���� (0:���, 1:�Ǻ�)
  		strData += "46308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
 		strData += "46309"; // ���ϸ���ȸ Input �ɺ�
 		strData += sDS;
 		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
 		strData += strTemp;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : ���� , 1: ��
			strData += "46341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ü� OutBound : KHD : IBK ���߱� 
						// �����ڵ�,  �̸�,  ���簡, ��������,����, �����, �����ŷ���
	char* pFOSymbol[] = { "41301", "41022", "41023", "41336", "41024", "41033", "41027",
						// ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,    	// ���Ͻð�, ���ϰ�, ��������, �����ŷ����
						"41311", "41312", "41029", "41030", "41031","41320", "41321", "41322", "45028","41201", "41307", "41900", "41901" };
					
	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;//sFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// �׸��强 ������(�迭��, �ݺ�������) OutBound
	//////////////////////////////////////////////////////////////////////////

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
		nSize = sizeof(szTickRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldOption[i];
			strData += sCOL;
		}
		break;

	case 1:		// ��
		nSize = sizeof(szMinRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldOption[i];
			strData += sCOL;
		}
		break;

	case 2:		// ��
	case 3:		// ��
	case 4:		// ��
		nSize = sizeof(szDayRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldOption[i];
			strData += sCOL;
		}
		break;
	case 6:		// ��
		nSize = sizeof(szSecRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldOption[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}
char* CChartItemFO::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0604_OutRec1_EX stOut;
	//CString strSrc;
	//strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strSrc = (char*)pData;
	CString strNode, strCode, strLock;
	CString strPrice, strVolume, strVolAmt;
	double dOpen, dHigh, dLow, dPreOpen, dPreHigh, dPreLow, dMaxHigh, dMinLow, dPrice;
	double dPrePrice, dChRate=0.0;
	double dopeninterest = 0.0; //�̰��� ����  

	memset(&stOut, 0x20, sizeof(stOut));

// �����ڵ�,  �̸�,  ���簡, ��������,����, �����, �����ŷ���, ���Ѱ�,  ���Ѱ� ,  �ð�,  ��,  ����,
//{ "30301", "30022", "31023", "31356", "31024", "31033", "31027","30354", "30355", "30358", "30359", "30360", };

	strCode = CDataConverter::Parser(strSrc, sCELL); // �����ڵ�
	
	int nOption = 0;
	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // ���簡

	// �����ڵ�� ���������� ���͵� �������弱���� ������ ��޵Ǿ� ���̽��� ����ó��.(���н��÷���...)
	// �������� ������ ��� ���ڸ��� ��ȣ�� ������ ��Ÿ�� (+, -, 0)���/�϶�/����
	// �������� ���� ���簡�� ������ ���� �� ����. ���ڸ� ��ȣ�� ���� ���簡���� ��ȣ.
	// ���/�϶� ������ ����� ��ȣ�� ����ϹǷ� ���� ó�� ����.
	// 	if( strPrice.Find("0-") > -1 || strPrice.Find("0+") > -1 || strPrice.Find("--") > -1 || strPrice.Find("-+") > -1 ||
	// 		strPrice.Find("+-") > -1 || strPrice.Find("++") > -1)	
	if( !strPrice.IsEmpty() )
		strPrice = strPrice.Mid( 1 );

	dPrice = atof(strPrice);//KHD
	strPrice.Remove(_T('+'));
	strPrice.Remove(_T('.'));
	int nPrice = atoi(strPrice);
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

//KHD : �ڵ�Ÿ�Կ� ���� ���ذ� ������ Ʋ����.
//	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
//	int nCodeType = pMasterDataManager->GetCodeTypeFromCode(m_strCode);

	strNode = CDataConverter::Parser(strSrc, sCELL);
	//���ذ� 
	if(m_nCodeType == CDRCodeTypeInfo::CODE_FUOPT_COMMODITY)
	{
		dPrePrice = (atof(strNode))/100;
	}
	else
	{
		dPrePrice = atof(strNode);	// ��������
		strNode.Format(_T("%d"), dPrePrice*100);
	}
//END 
	memcpy(stOut.preprice, strNode, min(sizeof(stOut.preprice), strNode.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // ��������
	strNode = strNode.Mid(1);
	strNode.Remove(_T('.'));
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // �����
	dChRate = atof(strNode);
	strNode.Remove(_T('+'));
	strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // �ŷ���
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	// Make sign value
	if (m_nCodeType == CDRCodeTypeInfo::CODE_FUOPT_COMMODITY)
	{
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMaxHigh = atof(strNode); // ���Ѱ�
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMinLow = atof(strNode); // ���Ѱ�
	}
	else
	{
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMaxHigh = fabs(atof(strNode)); // ���Ѱ�
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMinLow = fabs(atof(strNode)); // ���Ѱ�
	}

	if(abs(nPrice) == (int)(dMaxHigh*100))		stOut.sign[0] = '1'; // UP_LIMIT
	else if(abs(nPrice) == (int)(dMinLow*100))	stOut.sign[0] = '4'; // DOWN_LIMIT
	else if(dChRate < 0)						stOut.sign[0] = '5'; // DOWN
	else if(dChRate == 0)						stOut.sign[0] = '3'; // SAME (BOHAP)
	else										stOut.sign[0] = '2'; // UP

	strNode = CDataConverter::Parser(strSrc, sCELL);	dOpen = atof(strNode.Mid(1));		// �ð�
	strNode = CDataConverter::Parser(strSrc, sCELL);	dHigh = atof(strNode.Mid(1));		// ��
	strNode = CDataConverter::Parser(strSrc, sCELL);	dLow  = atof(strNode.Mid(1));		// ����
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreOpen = atof(strNode);	// ���Ͻð�
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreHigh = atof(strNode);	// ���ϰ�
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreLow  = atof(strNode);	// ��������

	//>>[A00000651][A00000652]�����ŷ����.
	strNode = CDataConverter::Parser(strSrc, sCELL);
	strVolAmt = strNode;
	strVolAmt += _T("00000000");
	//<<[A00000651][A00000652]
	//KHD : �̰��� ���� 
	strNode = CDataConverter::Parser(strSrc, sCELL);
	memcpy(stOut.openinterest, strNode, min(sizeof(stOut.openinterest), strNode.GetLength()));

	CString strRealState = CDataConverter::Parser(strSrc, sCELL);   // ���� ������(31307, 41307)  	           
																	// 00 : �尳����                  
																	// 10 : �ð����ϰ�                
																	// 11 : �ð����ϰ� ����           
																	// 20 : ���ߴ��ϰ�                
																	// 21 : ���ߴ��ϰ�����            
																	// 30 : �������ϰ�                
																	// 40 : ����                      
																	// 80 : �����Ÿ�ü��(�ֽİ��û�ǰ)
																	// 90 : �ŷ�����                  
																	// 99 : ������                    
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL);   // ���ð�����(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL);   // ���ð�����(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();
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
		nSize = sizeof(szTickRowFieldFO)/sizeof(char*);
	else if(nType == 1) //��
		nSize = sizeof(szMinRowFieldFO)/sizeof(char*);
	else if(nType == 6) //��
		nSize = sizeof(szSecRowFieldFO)/sizeof(char*);
	else		// ��, ��, ��
		nSize = sizeof(szDayRowFieldFO)/sizeof(char*);
	// �׸��� ���


 	GRAPH_IO gridOut;
 	strNode = strSrc.Left(sizeof(gridOut));
 	
 	memcpy(&gridOut, strNode, sizeof(gridOut)); 
 	
 	strSrc = strSrc.Mid(sizeof(gridOut));
 
	if (strNextKey == "99999999999999" || atoi(strNextKey) == 0)	// 9�� 14�ڸ��� ����Ű�� ���°�.
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));

	strSrc.Remove('.');
	
	CStringArray arrNode;
	int nNodeIndex = 1;
	strSrc.TrimLeft(); strSrc.TrimRight();

	int nEnd = strSrc.Find(sFS);
	if(nEnd !=-1 )
	{
		strSrc.Delete(nEnd +1, strSrc.GetLength() - nEnd);
	}
	else
	{	
		strSrc+=sCELL;
		strSrc+=sROW;
		strSrc+=sFS;
		strSrc+=sCELL;
	}
	
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);

		if(strNode != sROW && strNode != sCELL)
			arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// ��ü ������ �� ���
	nCnt = arrNode.GetSize()/ nSize;
//	nCnt	=	atoi(CString(gridOut.nrec, sizeof(gridOut.nrec)));//CString(stFirstBong.close, sizeof(stFirstBong.close))
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// ��Ʈ �� ������
	int nBongSize = sizeof(KB_p0604_OutRec2);
	int nBufSize = sizeof(KB_p0604_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0604_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate , strTime;
	char* pDataBuf = new char[nBufSize+1];
	memset(pDataBuf, 0x00, sizeof(nBufSize+1));

	KB_p0604_OutRec2 stFirstBong, stBong;
	double dValue=0;
	int nIndex = 0;
	int nRow = 0;
	for ( nRow = 0; nRow < nCnt; nRow++)
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
		if(strNode.GetLength() <3) continue;

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
		//@Solomon	091202SM123	-->
		// �ŷ����
		strNode = arrNode.GetAt(nIndex++);
		//dValue = atof(strNode)*100000000;	//[A00000408]�ŷ���� 12 -> 18. (6 -> 8��)
		//strNode.Format(_T("%.0f"), dValue);
		if(nType < 2 || nType == 6)	//ƽ�ʺ�.
		{
		}
		else
		{
			strNode += "00000000";
		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--
		//�̰������� 
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.openinterest, strNode, min(sizeof(stBong.openinterest), strNode.GetLength()));

		// ������ ���ۿ� ����, ���ֿ��� ��,��,ƽ�� ������ �ݴ�
		//	memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);//KHD : �ݴ�� �ͼ� �ݴ�� ���� 
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

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
		case 1:	dDenom = nFirstClose;			break;	// ù���� ����
		case 2:	dDenom = 1;						break;	// $   ������ ó������
		case 3:	dDenom = dOpen*100;		break;	// ���Ͻð�
		case 4:	dDenom = nPrice;				break;	// ���簡
		case 5:	dDenom = dPrePrice*100;	break;	// ��������
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
//	m_bojoInput.m_sPrice = strPrice;		// ���簡
	m_bojoInput.m_sPrice.Format(_T("%.2f"), dPrice);	// ���簡 
	m_bojoInput.m_sVolume = strVolume;		// �ŷ���
// 		m_bojoInput.m_sUpLmtPrice.Format(_T("%.0f"), dMaxHigh*100);	// ���Ѱ�
// 		m_bojoInput.m_sDnLmtPrice.Format(_T("%.0f"), dMinLow*100);	// ���Ѱ�
// 		m_bojoInput.m_sOpen.Format(_T("%.0f"), dOpen*100);		// �ð�
// 		m_bojoInput.m_sHigh.Format(_T("%.0f"), dHigh*100);		// ��
// 		m_bojoInput.m_sLow.Format(_T("%.0f"), dLow*100);			// ����
// 		//@Solomon:091123SM068 -->
// 		m_bojoInput.m_sPreOpen.Format(_T("%.0f"), dPreOpen*100);		// ���Ͻð�
// 		m_bojoInput.m_sPreHigh.Format(_T("%.0f"), dPreHigh*100);		// ���ϰ�
// 		m_bojoInput.m_sPreLow.Format(_T("%.0f"), dPreLow*100);		// ��������
// 		//@Solomon:091123SM068 <--
// 		m_bojoInput.m_sPreprice.Format(_T("%.0f"), dPrePrice*100);	// ��������
	m_bojoInput.m_sUpLmtPrice.Format(_T("%.2f"), dMaxHigh);	// ���Ѱ�
	m_bojoInput.m_sDnLmtPrice.Format(_T("%.2f"), dMinLow);	// ���Ѱ�
	m_bojoInput.m_sOpen.Format(_T("%.2f"), dOpen);		// �ð�
	m_bojoInput.m_sHigh.Format(_T("%.2f"), dHigh);		// ��
	m_bojoInput.m_sLow.Format(_T("%.2f"), dLow);			// ����
	//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen.Format(_T("%.2f"), dPreOpen);		// ���Ͻð�
	m_bojoInput.m_sPreHigh.Format(_T("%.2f"), dPreHigh);		// ���ϰ�
	m_bojoInput.m_sPreLow.Format(_T("%.2f"), dPreLow);		// ��������
	//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice.Format(_T("%.2f"), dPrePrice);	// ��������
//	m_bojoInput.m_sPreVolume = strPreVolume;	// ���ϰŷ���
	m_bojoInput.m_sStartTime = strResetTime;	// �ŷ����۽ð�(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// ��ȸ�� ù���� ����.
	m_bojoInput.m_sFirstClose.TrimRight();

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
	sprintf(szKey, _T("%03d"), 30); // 103�� ����

	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode, nOption);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));
	return pDataBuf;
}
