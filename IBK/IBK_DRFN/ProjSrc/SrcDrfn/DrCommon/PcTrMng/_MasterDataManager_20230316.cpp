#include "stdafx.h"
#include "PcTrMng.h"

//--> For Solomon
#include "../../../SrcSite/include/axisfire.h"	
#include "../../inc/IAUPartnerMng.h"
//<-- For Solomon

#define	getCODE			0x45		// char* GetCode(_codeR*)
						// lParam : _codeR*

char *g_pszCFTiesCode[] = { "16599999", "16699999", "16799999", "17599999", "17699999", "17799999" };
char *g_pszCFTiesName[] = { "��3�ⱹä", "��5�ⱹä", "��10�ⱹä", "�̱��޷�", "��", "����" };

char *g_pszSFTiesCode[] = { "11199999", "11299999", "11399999", "11499999", "11599999", "11699999",
							"11899999", "11999999", "12199999", "12499999", "13299999", "13699999",
							"13999999", "14099999", "14199999", "14499999", "14599999", "14699999",
							"14899999", "14999999", "15099999", "15199999", "15299999", "15499999",
							"15699999", "15799999" };

char *g_pszSFTiesName[] = { "�Ｚ����", "SK�ڷ���", "POSCO", "KT", "�ѱ�����", "������",
							"��������", "�����", "�Ｚ����", "LG����", "������ö", "����Ƽ����",
							"�����߰���", "�ϳ�����", "SK�̳뺣��", "�츮����", "LGD", "KB����",
							"�������", "�λ�������", "SK���̴н�", "GS�Ǽ�", "NHN", "�̸�Ʈ",
							"NAVER", "�����װ�" };

//�����ƾ--> ����������... �ʿ�� ���.
// void _DoAlzioSerialize(LPCSTR szFileKey, 
// 					   CStringArray* parCode, CStringArray* parName, CStringArray* parEngName)
// {
// 	CString szLogFile;
// 	szLogFile.Format("%s\\master\\%s.dat", theApp.m_MainInfoMng.GetDataDir(), szFileKey);
// 	
// 	CFile file;
// 	if (file.Open(szLogFile, CFile::typeBinary|CFile::modeRead))
// 		//if (file.Open(szLogFile, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
// 	{
// 		CArchive ar(&file, CArchive::load);
// 		//CArchive ar(&file, CArchive::store);
// 		
// 		int nVer = 1;
// 		if (ar.IsStoring())
// 		{	// storing code
// 			ar << nVer;
// 			parCode->Serialize(ar);
// 			parName->Serialize(ar);
// 			parEngName->Serialize(ar);
// 		}
// 		else
// 		{	// loading code
// 			ar >> nVer;
// 			parCode->Serialize(ar);
// 			parName->Serialize(ar);
// 			parEngName->Serialize(ar);
// 		}
// 		ar.Close();
// 		file.Close();
// 	}
// }
//<--�����ƾ
// 	CStringArray gFOCode, gFOCodeName;
// 	CMapStringToString gFOCodeMap;
// 	void _gDoReadFOMaster(CStringArray &arrCode, CStringArray &arrName)
// 	{
// 		CString szLogFile;
// 		szLogFile.Format("%s\\master\\%s.dat", theApp.m_MainInfoMng.GetDataDir(), "FOMaster");
// 
// 		if(gFOCode.GetSize()==0)
// 		{
// 			CStdioFile file;
// 			if (file.Open(szLogFile, CFile::typeText|CFile::modeRead))
// 			{
// 				CString rString, rCode, rCodeName;
// 				while(file.ReadString(rString))
// 				{
// 					rCode = rString.Left(10);
// 					rCodeName = rString.Mid(11);
// 
// 					gFOCode.Add(rCode);
// 					gFOCodeName.Add(rCodeName);
// 
// 					gFOCodeMap.SetAt(rCode, rCodeName);
// 				}
// 				file.Close();
// 			}
// 		}
// 
// 		for(int i=0; i<gFOCodeName.GetSize(); i++)
// 		{
// 			arrCode.Add(gFOCode.GetAt(i));
// 			arrName.Add(gFOCodeName.GetAt(i));
// 		}
// 	}
// ----------------------------------------------------------------------
// CMasterDataManager
CMasterDataManager::CMasterDataManager()
{
// 	m_nJ_K_Count = 0;
// 	m_nJ_K_Count_J = 0;
// 	m_nJ_K_Count_K = 0;

//@Solomon090819	m_itemMaster.Load();
//@Solomon090819	m_itemMaster.LoadThemeInfo();

//	m_hWinixWnd = NULL;
	m_hCodeMasterWnd = NULL;
}

CMasterDataManager::~CMasterDataManager()
{
//	UnLoadMASTER_J_K();
	ClearListSTRINGBYTE(&m_RecentCodeTypeList);
	ClearListSTRINGBYTE(&m_RecentMarketTypeList);
	ClearListSTRINGSTRING(&m_RecentCodeNameList);
	ClearListSTRINGSTRING(&m_RecentChxUpCodeList);
}

void CMasterDataManager::ClearListSTRINGBYTE(List_ST_STRINGBYTE* pList)
{
	ST_STRINGBYTE* pItem;
	for(POSITION pos=pList->GetHeadPosition(); pos;)
	{
		pItem = pList->GetNext(pos);
		if(pItem) delete pItem;
	}
	pList->RemoveAll();
}

void CMasterDataManager::ClearListSTRINGSTRING(List_ST_STRINGSTRING* pList)
{
	ST_STRINGSTRING* pItem;
	for(POSITION pos=pList->GetHeadPosition(); pos;)
	{
		pItem = pList->GetNext(pos);
		if(pItem) delete pItem;
	}
	pList->RemoveAll();
}

void CMasterDataManager::SetKeyData(LPCSTR szKey, LPCSTR szData)
{	
}

void CMasterDataManager::SetKeyData(LPCSTR szKey, long   dwData)
{
// 	if(strcmp(szKey, "WinixWnd")==0)
// 	{
// 		m_hWinixWnd = (HWND)dwData;
// 		if(!m_hWinixWnd || !IsWindow(m_hWinixWnd))
// 			m_hWinixWnd = NULL;
// 	}
}

HWND CMasterDataManager::GetHWND()
{
	HWND hWnd = NULL;
	IPartnerSvrManager* pMng = (IPartnerSvrManager*)theApp.m_GateMng.GetInterface(UUID_IPartnerSvrManager);
	if(pMng)
		pMng->GetParentHandle(0, hWnd);
	
	return hWnd;
}
//@Solomon
HWND CMasterDataManager::GetCodeMasterHWND()
{
	if(m_hCodeMasterWnd==NULL)
	{
		HWND hWnd = theApp.m_pPctrWinixLoader->PTM_GetCodeMaster();
		m_hCodeMasterWnd = hWnd;
		return hWnd;
	}
	return m_hCodeMasterWnd;
}

// {{ Code.ocx
long CMasterDataManager::GetBaseCodeTable()
{
	//@solomon return m_CodeOcx.GetBaseCodeTable();
	return NULL;
}

void CMasterDataManager::SetInfo(long _dwInfo)
{
	//@solomonm_CodeOcx.SetInfo(_dwInfo);
}

long CMasterDataManager::ShowCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd)
{
	//@solomon return m_CodeOcx.ShowCodeTable(_szCodeKey, _hWnd);
	return NULL;
}

long CMasterDataManager::ShowModeCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd)
{
	//@solomon return m_CodeOcx.ShowModeCodeTable(_szCodeKey, _hWnd);
	return NULL;
}

void CMasterDataManager::SetDataString(short nKey, LPCTSTR szData)
{
	//@solomon m_CodeOcx.SetDataString(nKey, szData);
}

BOOL CMasterDataManager::InitData(LPCTSTR szRootPath)
{
	//@solomon return m_CodeOcx.InitData(szRootPath);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.08.29
// �Լ� �̸�         : CMasterDataManager::GetCodeInfo
// ���� Ÿ��         : CString
// ���� Ÿ�� ����    : 
// �Ķ����			 :	LPCTSTR _szCodeKey;		// MarketKey
//						short _nFlag;			// * _nFlag 0 : �ڵ尪���� �ڵ�� ��ȸ
												// *		1 : �ڵ������ �ڵ尪 ��ȸ
												// *		2 : �ڵ屸�� ����	
//						LPCTSTR _szKey;			// ���� �ڵ�	
// �Լ� ����         : �Ｚ CodeMaster���� �������񸶽��� �����͸� ����
///////////////////////////////////////////////////////////////////////////////// 
CString CMasterDataManager::GetCodeInfo(LPCTSTR _szCodeKey, short _nFlag, LPCTSTR _szKey)
{
	switch(_nFlag)
	{
		case 0:
			return GetCodeName(_szKey);
		case 1:
			AfxMessageBox("CMasterDataManager::GetCodeInfo 1 ó���ȵ�");
			return "";
		case 2:
			{
				int nMarketType = GetMarketTypeFromCode(_szKey);
				CString strData;
				strData.Format("%d", nMarketType);
				return strData;
			}
		default:
			break;
	}
	
//@�ſ�����
//	CString strCode = _szKey;		// �����ڵ� (�Է�) 
//	CString strMarketKey = _szCodeKey;
//	int	nFlag = _nFlag;
//
//	BYTE cType;
//	CString strType;
//
//	switch(nFlag)
//	{
//	// �����ڵ带 ���� ������� ��������
//	// GetJongmokNameFromCode �Լ� �̿�
//	case 0:			// �ڵ尪���� �ڵ�� ��ȸ
//		if(strMarketKey == "UMASTER")		// ����
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_ALLUPJONG, strCode);
//		else if(strMarketKey == "PMASTER")	// �ɼ�
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_OPTION, strCode);
//		else if(strMarketKey == "FMASTER")	// ����
//		{
//			CString strName = dll_GetJongmokName(CODE_FUTURE, strCode);
//			if(strName.GetLength()<1)
//			{
//				strName = dll_GetJongmokName(CODE_STARFUT, strCode);
//			}
//			theApp.m_szTemp = strName;
//		}
//		else if(strMarketKey == "FRMASTER")	// �ؿ�
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_FRMJISU, strCode);
//		else if(strMarketKey == "WMASTER")	// ELW
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_ELW, strCode);
//		else								// ����
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_ALLJONGMOK, strCode);
//		return theApp.m_szTemp;
//		break;
//	// �����ڵ带 ���� ���屸���� ��������
//	// GetMarketTypeFromCode �Լ� �̿�
//	case 2:		// �ڵ尪���� �ڵ屸�� ����
//				//	"1"		= 	���� ���� 
//				//	"2"		= 	�ڽ��� ����
//				//	"3"		= 	�������� 
//				//	"4"		= 	����, �������� 				
//				//	"5"		= 	�ɼ� 						
//				//	"8"		= 	ä�� 						
//				//	"9"		= 	�������� 					
//				//	"A"		= 	ETF�ڵ� 					
//				//	"B"		= 	KOSPI 200 ����, KOSPI 100 ����, �峻����, KRX, KRX����
//				//	"C"		= 	�ڽ��� ����, �ڽ��� 50 		
//				//	"G"		= 	�����ֽĿɼ�  				
//				//	"L"		= 	����������� 				
//				//	"M"		= 	��Ÿ���� 					
//				//	"N"		=	�ֽĿ���Ʈ ELW 	
//		cType = dll_GetJongMarket((LPCTSTR)(LPCSTR)strCode);
//		theApp.m_szTemp = dll_GetChangeMarketCode(cType, false);
//		return theApp.m_szTemp;
//	default:
//		break;
//	}

//@Solomon090819	char cMarketType = m_itemMaster.GetItemType((LPCSTR)_szKey);
//@Solomon090819	switch(cMarketType)
//@Solomon090819	{
//@Solomon090819	case IMT_KOSPI_STOCK:	return "1";
//@Solomon090819	case IMT_KOSDAQ_STOCK:	return "2";
//@Solomon090819	case IMT_ELW:			return "N";
//@Solomon090819	case IMT_KOSPI_UPJONG:	return "B";
//@Solomon090819	case IMT_KOSDAQ_UPJONG:	return "C";
//@Solomon090819	case IMT_KFREEBOARD:	return "3";
//@Solomon090819	case IMT_KFUTURE:
//@Solomon090819	case IMT_FUTURE_SPREAD:	return "4";
//@Solomon090819	case IMT_KOPTION:		return "5";
//@Solomon090819	}

	return "";
}

void CMasterDataManager::SetPosition(LPCTSTR szKey, short nX, short nY)
{
	//@solomon m_CodeOcx.SetPosition(szKey, nX, nY);
}

CString CMasterDataManager::GetMaxHangSaGa()
{
	//@solomon return m_CodeOcx.GetMaxHangSaGa();
	return "";
}

long CMasterDataManager::ShowCodeTableWithPosition(LPCTSTR _szCodeKey, const VARIANT& _hWnd, short nX, short nY)
{
	//@solomon return m_CodeOcx.ShowCodeTableWithPosition(_szCodeKey, _hWnd, nX, nY);
	return 0;
}

BOOL CMasterDataManager::RemoveAllListData()
{
	//@solomon return m_CodeOcx.RemoveAllListData();
	return TRUE;
}

BOOL CMasterDataManager::InsertHistory(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName)
{
	//@solomon return m_CodeOcx.InsertHistory(nMaxCount, nType, Code, CodeName);
	return TRUE;
}

CString CMasterDataManager::GetHistoryList(short nMaxCount, long nType)
{
	//@solomon return m_CodeOcx.GetHistoryList(nMaxCount, nType);
	return "";
}

long CMasterDataManager::GetGlobalDataTypeList()
{
	//@solomon return m_CodeOcx.GetGlobalDataTypeList();
	return 0;
}

long CMasterDataManager::GetCodePointer(LPCTSTR _szCodeKey)
{
	//@solomon return m_CodeOcx.GetCodePointer(_szCodeKey);
	return 0;
}
// }} Code.ocx


// �ֽ����񸶽��Ϳ��� ������ �ش��ϴ� �����ϱ�
// ���ϵǴ� ���´� �����ͱ��ϴ� ��������
// 	int CMasterDataManager::GetJMaster_Upcode(LPCTSTR szUpcode, long pReturnList)
// 	{
// 		CList<CJMaster, CJMaster>* pRList = (CList<CJMaster, CJMaster>*)pReturnList;
// 		CList<CJMaster, CJMaster>* list = (CList<CJMaster, CJMaster>*)GetCodePointer("JMASTER"); 
// 
// 		int i=0;
// 		for(POSITION pos=list->GetHeadPosition(); pos;)
// 		{
// 			CJMaster data = list->GetNext(pos);
// 			if(data.m_szS.Compare(szUpcode)==0)
// 			{
// 				pRList->AddTail(data); i++;
// 			}
// 		}
// 		return i;
// 	}
// 
// 	// �ڽ������񸶽��Ϳ��� ������ �ش��ϴ� �����ϱ�
// 	// ���ϵǴ� ���´� �����ͱ��ϴ� ��������
// 	int CMasterDataManager::GetKMaster_Upcode(LPCTSTR szUpcode, long pReturnList)
// 	{
// 		CList<CKMaster, CKMaster>* pRList = (CList<CKMaster, CKMaster>*)pReturnList;
// 		CList<CKMaster, CKMaster>* list = (CList<CKMaster, CKMaster>*)GetCodePointer("KMASTER"); 
// 
// 		int i=0;
// 		for(POSITION pos=list->GetHeadPosition(); pos;)
// 		{
// 			CKMaster data = list->GetNext(pos);
// 			if(data.m_szUpCodel.Compare(szUpcode)==0)
// 			{
// 				pRList->AddTail(data); i++;
// 			}
// 		}
// 		return i;
// 	}


// BOOL CMasterDataManager::IsKOSPI(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsKosdaq(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsStock(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsFuture(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsSpread(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsStockOption(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsOption(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsCall(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsPut(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsKP200(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsKQ50(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::CheckKP200Sect(LPCSTR szCode)
// {
// 	return FALSE;
// }

// int CMasterDataManager::LoadMASTER_J_K()
// {
// 	return m_nJ_K_Count;
// }
// 
// void CMasterDataManager::UnLoadMASTER_J_K()
// {
// 	m_nJ_K_Count = 0;
// 	m_nJ_K_Count_J = 0;
// 	m_nJ_K_Count_K = 0;
// }
// 
// int CMasterDataManager::J_K_JMasterCount()
// {
// 	return m_nJ_K_Count_J;
// }
// 
// int CMasterDataManager::J_K_KMasterCount()
// {
// 	return m_nJ_K_Count_K;
// }
//
// int CMasterDataManager::_SortName(const void* p1, const void* p2)
// {
// 	CJMaster *pChild1 = *(CJMaster**)p1;
// 	CJMaster * pChild2 = *((CJMaster**)p2);
// 
// 	return pChild1->m_szName.Compare(pChild2->m_szName);
// }
// 
// int CMasterDataManager::_SortCode(const void* p1, const void* p2)
// {
// 	CJMaster *pChild1 = *(CJMaster**)p1;
// 	CJMaster * pChild2 = *((CJMaster**)p2);
// 
// 	return pChild1->m_szCode.Compare(pChild2->m_szCode);
// }
// 
// 
// void CMasterDataManager::CodeSort(LPCSTR szMaster, void* pList, int nCount)
// {
// }
// 
// 
// void  CodeSort_JMaster(void* pList, int nCount)
// {
// }

///////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.08.28
// �Լ� �̸�         : CMasterDataManager::GetOptionJongMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrJongCode;	// �ɼ� ���� �ڵ� 
//						CStringArray* pastrJongName;	// �ɼ� ���� �� 	
//						CStringArray* pastrJongEngName;	// �ɼ� ���� ���� ��	
//						CStringArray* pastrEventPrice;	// ��� ����	
//						CStringArray* pastrFinalYM;		// ���� ���	
//						CStringArray* pastrValueGubun;	// ���� ����	
// �Լ� ����         : �Ｚ CodeMaster���� �ɼ����񸶽��� �����͸� ����
///////////////////////////////////////////////////////////////////////////////// 
BOOL CMasterDataManager::GetOptionJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName, 
										  CStringArray *&pastrJongEngName, CStringArray *&pastrEventPrice,	
                                          CStringArray *&pastrFinalYM,	CStringArray *&pastrValueGubun	  )
{
	return GetMasterDataInUpDetails("PMASTER", *pastrJongCode, *pastrJongName, "");
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_OPTION_K200;
	//codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	//_DoParsingCode(strRet, *pastrJongCode, *pastrJongName, true);
	_DoParsingCodePMaster(strRet, *pastrJongCode, *pastrJongName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOPTION, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrJongCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrJongName->Add(str);
// 	}
//@Solomon090819 <--
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.08.28
// �Լ� �̸�         : CMasterDataManager::GetFutureJongMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrJongCode;	// ���� �ڵ� 
//						CStringArray* pastrJongName;	// �ѱ� ���� �� 	
//						CStringArray* pastrJongEngName;	// ���� ���� ��	
// �Լ� ����         : �Ｚ CodeMaster���� �������񸶽��� �����͸� ����
///////////////////////////////////////////////////////////////////////////////// 
BOOL CMasterDataManager::GetFutureJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName, CStringArray *&pastrJongEngName)
{
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_FUTURE_K200;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, *pastrJongCode, *pastrJongName);
	pastrJongCode->Add(DEF_CONTINUE_FUT_CODE);
 	pastrJongName->Add(SZDEF_CONTINUE_FUT_CODE);

//@Solomon090819-->
// 	__int64 nType64 = 0;
// 	nType64 |= __int64(1) << KFUTURES_MASTER;
// 	nType64 |= __int64(1) << KOSPI_FUTURE_SPREAD;
// //	nType64 |= __int64(1) << KFUTURES_LINK;
// 
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData2(nType64, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrJongCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrJongName->Add(str);
// 	}
// 	
// 	pastrJongCode->Add("99999999");
// 	pastrJongName->Add("���ἱ��");
//@Solomon090819<--
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.07.05
// �Լ� �̸�         : CMasterDataManager::GetKSPUpMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrUpCode;		// ���� �ڵ� 
//						CStringArray* pastrUpName;		// �ѱ� ���� �� 	
//						CStringArray* pastrUpEngName;	// ���� ���� ��	
// �Լ� ����         : KSP ���� ������
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKSPUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KOSPI;
	else			codeR.kind = CDKIND_INDEX_KOSPI;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.07.05
// �Լ� �̸�         : CMasterDataManager::GetKSQUpMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrUpCode;		// ���� �ڵ� 
//						CStringArray* pastrUpName;		// �ѱ� ���� �� 	
//						CStringArray* pastrUpEngName;	// ���� ���� ��	
// �Լ� ����         : �ڽ��� ���� ������
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKSQUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KOSDAQ;
	else			codeR.kind = CDKIND_INDEX_KOSDAQ;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.07.05
// �Լ� �̸�         : CMasterDataManager::GetKNXUpMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrUpCode;		// ���� �ڵ� 
//						CStringArray* pastrUpName;		// �ѱ� ���� �� 	
//						CStringArray* pastrUpEngName;	// ���� ���� ��	
// �Լ� ����         : �ڳؽ� ���� ������
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKNXUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KNX;
	else			codeR.kind = CDKIND_INDEX_KNX;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.09.13
// �Լ� �̸�         : CMasterDataManager::GetFRMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrCode;		// �ؿ��ֿ����� �ɹ�
//						CStringArray* pastrName;		// �ѱ����� �� 	
// �Լ� ����         : �ؿ��ֿ����� ������	CODE_FRMAJORIDX
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetFRMst(CStringArray *&pastrCode, CStringArray *&pastrName)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.09.13
// �Լ� �̸�         : CMasterDataManager::GetFRNationMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrCode;		// ���� �ڵ� 
//						CStringArray* pastrName;		// �ѱ� ���� �� 	
// �Լ� ����         : �ؿܱ��� ������	CODE_FRNATION
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetFRNationMst(CStringArray *&pastrCode, CStringArray *&pastrName)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.09.13
// �Լ� �̸�         : CMasterDataManager::GetELWMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrCode;		// ���� �ڵ� 
//						CStringArray* pastrName;		// �ѱ� ���� �� 	
// �Լ� ����         : ELW ������	CODE_ELW
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetELWMst(CStringArray *&pastrCode, CStringArray *&pastrName)
{
//@�ſ�����	return dll_GetMasterData(CODE_ELW , *pastrCode, *pastrName);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.09.13
// �Լ� �̸�         : CMasterDataManager::GetGroupMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrCode;		// �׷� �ڵ� 
//						CStringArray* pastrName;		// �ѱ� �׷� �� 	
// �Լ� ����         : ELW ������	CODE_ELW
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetGroupMst(CStringArray *&pastrCode, CStringArray *&pastrName, int nOption)
{
	pastrCode->RemoveAll();
	pastrName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_SUBSIDIARY;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrCode, *pastrName);

//@Solomon090819 -->
// 	// 0 : �ŷ���, 1 : ���
// 	int nCount = m_itemMaster.GetGroupList2(1, *pastrCode, *pastrName);
// 	if(nCount <= 0)	return FALSE;

//@Solomon090819<--
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2006.07.05
// �Լ� �̸�         : CMasterDataManager::GetKSQUpMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrUpCode;		// ���� �ڵ� 
//						CStringArray* pastrUpName;		// �ѱ� ���� �� 	
//						CStringArray* pastrUpEngName;	// ���� ���� ��	
// �Լ� ����         : KRX ���� ������
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKRXUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KRX;
	else			codeR.kind = CDKIND_INDEX_KRX;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2007.01.11
// �Լ� �̸�         : CMasterDataManager::GetStockDetailInfo
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 : CString strCode;		// �����ڵ�
// �Լ� ����         : ���������
/////////////////////////////////////////////////////////////////////////////////////////////
CString CMasterDataManager::GetStockDetailInfo(CString strCode, int nType)
{
	return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2007.01.25
// �Լ� �̸�         : CMasterDataManager::GetLastCode
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 : CString strMarketCode;		// �����ڵ� 01:�ֽ�
// �Լ� ����         : �����丮 ������ �ڵ�
/////////////////////////////////////////////////////////////////////////////////////////////
CString CMasterDataManager::GetLastCode(CString strMarketCode)
{
	if(atoi(strMarketCode) < 0 || atoi(strMarketCode) > 6)
	{
		strMarketCode = "01";
	}
	
	int nHistoryCount = 0;
	CString strRecentCode;

	return strRecentCode;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2007.04.16
// �Լ� �̸�         : CMasterDataManager::GetK200UpMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrUpCode;		// ���� �ڵ� 
//						CStringArray* pastrUpName;		// �ѱ� ���� �� 	
//						CStringArray* pastrUpEngName;	// ���� ���� ��	
// �Լ� ����         : KSP ���� ������
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetK200UpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_K200;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOSPI200_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}
//@Solomon090819 <--
	return TRUE;
}

BOOL CMasterDataManager::GetK100UpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_K100;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

//@Solomon090819 -->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOSPI100_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}

//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetKSTRUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_KOSTAR;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	//@Solomon090819 -->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOSTAR_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}
	//@Solomon090819 <--

	return TRUE;
}

BOOL CMasterDataManager::GetFREEUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)
		codeR.kind = CDKIND_FREEBOARD;
 	else
 		codeR.kind = CDKIND_INDEX_FREEBOARD;

	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	
	//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_FREEBOARD_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}
	//@Solomon090819 <--

	return TRUE;
}

BOOL CMasterDataManager::GetForeignMst(int nKind, CString strCode, CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if ((strCode.GetLength() > 0) && strCode.Compare(_T("NONE")))
		memcpy(codeR.code, strCode, min(sizeof(codeR.code), strCode.GetLength()));
	
	codeR.kind = nKind;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	//_DoParsingCodeForeign(strRet, *pastrUpCode, *pastrUpName);
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2010.03.16
// �Լ� �̸�         : CMasterDataManager::GetFXMst
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CStringArray* pastrUpCode;		// ���� �ڵ� 
//						CStringArray* pastrUpName;		// ���� �� 	
// �Լ� ����         :  FX ������
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetFXMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_GFX_INDEX;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCodeFX(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ���� ��¥         : 2007.12.24
// �Լ� �̸�         : CMasterDataManager::GetRecentCode
// ���� Ÿ��         : BOOL
// ���� Ÿ�� ����    : 
// �Ķ����			 :	CString szCode
//						CString& rszCode	
//						int nCodeType
// �Լ� ����         :  ����,�ɼ� �����ڵ� ����������� ��� Ȯ���� ���ؼ� ...
//						����� ������ FALSE�̰� rszCode�� ���� �ֱٿ��� ����.
//						������� ���������� TRUE�����ϰ� rszCode�� szCode�� �״�� �Ѱ���.
/////////////////////////////////////////////////////////////////////////////////////////////
//alzioyes:20110526 ���ἱ���϶� �ֱٿ����� ��ȯ�ϴ� ��ƾ ����.
BOOL CMasterDataManager::GetRecentCode(CString szCode, CString& rszCode, int nCodeType)
{
	if(m_RecentFutureCodeTable.Lookup(szCode, rszCode)==TRUE)
		return TRUE;

	BYTE rCodeType = GetCodeTypeFromCode(szCode);
	rszCode = szCode;

	if(rCodeType==CDRCodeTypeInfo::CODE_KFUTURE) //����
	{
		CStringArray* pastrJongCode;
		CStringArray* pastrJongName;	// �ɼ� ���� �� 
		CStringArray* pastrJongEngName;	// �ɼ� ���� ���� ��
		
		pastrJongCode = new CStringArray;
		pastrJongName = new CStringArray;
		pastrJongEngName = new CStringArray;
		CString strCodeSelected;
		if ( szCode.CompareNoCase("99999999") == 0 )
			GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
		else if ( szCode.CompareNoCase("10599999") == 0 )
			GetMiniFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
		
		
		if(pastrJongCode->GetSize()>0)
			rszCode = pastrJongCode->GetAt(0);
		
		delete pastrJongCode;
		delete pastrJongName;
		delete pastrJongEngName;
		
		m_RecentFutureCodeTable.SetAt(szCode, rszCode);
		return TRUE;
	}
	else if(rCodeType==CDRCodeTypeInfo::CODE_FUOPT_COMMODITY)	//��ǰ����.
	{
/*		CStringArray *pArrCode, *pArrName, *pArrEng;
		pArrCode = new CStringArray;
		pArrName = new CStringArray;
		pArrEng = new CStringArray;
		if (!GetForeignMst(CDKIND_KOFEXLIST, "", pArrCode, pArrName, pArrEng))
			return FALSE;

		for (int i=0; i<pArrCode->GetSize(); i++)
		{
			CStringArray arrDCode, arrDName;
			if (!GetMasterDataInUpDetails("��ǰ����", arrDCode, arrDName, pArrCode->GetAt(i)))
				continue;

			if (arrDCode.GetSize() > 0 && strncmp(arrDCode.GetAt(0), szCode, 3) == 0)
			{
				rszCode = arrDCode.GetAt(0);
				break;
			}
		}

		pArrCode->RemoveAll();		delete pArrCode;
		pArrName->RemoveAll();		delete pArrName;
		pArrEng->RemoveAll();		delete pArrEng;

		m_RecentFutureCodeTable.SetAt(szCode, rszCode);
*/
	}
	else if(rCodeType==CDRCodeTypeInfo::CODE_STOCKFUT)	//�ֽļ���.
	{
/*
		CStringArray arrCode, arrName;
		if (!GetJFMaster(arrCode, arrName))
			return FALSE;

		for (int i=0; i<arrCode.GetSize(); i++)
		{
			CStringArray arrDCode, arrDName;
			if (!GetMasterDataInUpDetails("JFMASTER", arrDCode, arrDName, arrCode.GetAt(i)))
				continue;

			if (arrDCode.GetSize() > 0 && strncmp(arrDCode.GetAt(0), szCode, 3) == 0)
			{
				rszCode = arrDCode.GetAt(0);
				break;
			}
		}

		m_RecentFutureCodeTable.SetAt(szCode, rszCode);
*/
	}
	
	return TRUE;
	
}

// BOOL CMasterDataManager::GetRecentCode(CString szCode, CString& rszCode, int nCodeType)
// {
// 	rszCode = szCode;
// 
// 	if(nCodeType==IMaster_Future) //����
// 	{
// 		CStringArray* pastrJongCode;
// 		CStringArray* pastrJongName;	// �ɼ� ���� �� 
// 		CStringArray* pastrJongEngName;	// �ɼ� ���� ���� ��
// 
// 		pastrJongCode = new CStringArray;
// 		pastrJongName = new CStringArray;
// 		pastrJongEngName = new CStringArray;
// 		CString strCodeSelected;
// 		GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
// 
// 		if(pastrJongCode->GetSize()>0)
// 			rszCode = pastrJongCode->GetAt(0);
// 
// 		delete pastrJongCode;
// 		delete pastrJongName;
// 		delete pastrJongEngName;
// 
// 		return TRUE;
// 	}
// 
// 	return TRUE;
// }

BOOL CMasterDataManager::GetMasterDataInUpDetails(CString _szType, CStringArray &arrCode, CStringArray &arrName, CString strKey)
{
	CString szType = _szType;
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));

	codeR.type = CDTYPE_CODE | CDTYPE_NAME;

	if (strKey.CompareNoCase(_T("NONE")))
		memcpy(codeR.code, (LPCSTR)strKey, strKey.GetLength());

	BOOL bEachCall = FALSE;

	if(!_szType.CompareNoCase("PMASTER"))
	{
		codeR.kind = CDKIND_OPTION_K200;
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		bEachCall = TRUE;
	}
	else if(!_szType.CompareNoCase("MINI_PMASTER"))
	{
		codeR.kind = CDKIND_MINI_OPTION;
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		bEachCall = TRUE;
	}
	else if(!_szType.CompareNoCase("WEEKLY_PMASTER"))
	{
		codeR.kind = CDKIND_WEEKLY_OPTION;
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		bEachCall = TRUE;
	}
	else if(!_szType.CompareNoCase("JFMASTER"))
	{
		// 2012.2.3 �ڼ���
		//codeR.code[0] = 'A';
		//memcpy(&codeR.code[1], (LPCSTR)strKey, strKey.GetLength());
		memcpy(codeR.code, (LPCSTR)strKey, strKey.GetLength());
		
		codeR.kind = CDKIND_SFUTURE;
	}
	else if(!_szType.CompareNoCase("JPMASTER"))
	{
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		codeR.kind = CDKIND_SOPTION;
		bEachCall = TRUE;
	}
	else if (!_szType.CompareNoCase("UMaster"))			codeR.kind = CDKIND_KOSPI;
	else if (!_szType.CompareNoCase("kumASTER"))		codeR.kind = CDKIND_KOSDAQ;
	else if (!_szType.CompareNoCase("TMMASTER"))		codeR.kind = CDKIND_THEME;
	else if (!_szType.CompareNoCase("ETFMASTER"))		codeR.kind = CDKIND_ETF;
//	else if (!_szType.CompareNoCase("FREEJMASTER"))		codeR.kind = CDKIND_FREEBOARD;
	else if (!_szType.CompareNoCase("KRXUMaster"))		codeR.kind = CDKIND_KRX;
	else if (!_szType.CompareNoCase("KNXUMaster"))		codeR.kind = CDKIND_KNX;
	else if (!_szType.CompareNoCase("K200UMaster"))		codeR.kind = CDKIND_INDEX_K200;
	else if (!_szType.CompareNoCase("K100UMaster"))		codeR.kind = CDKIND_INDEX_K100;
	else if (!_szType.CollateNoCase("GRPMASTER"))		codeR.kind = CDKIND_SUBSIDIARY;
	else if (!_szType.CompareNoCase("ELWINDEXMASTER"))	codeR.kind = CDKIND_ELW_BASE;
	else if (!_szType.CompareNoCase("ELWISSUEMASTER"))	codeR.kind = CDKIND_ELW_PB;
	else if (!_szType.CompareNoCase("KOSPI200"))		codeR.kind = CDKIND_KOSPI200;
	else if (!_szType.CompareNoCase("OLDFMASTER"))		return _DoGetOldFutureMaster(arrCode, arrName);
	else if (!_szType.CompareNoCase("ETNMASTER"))		codeR.kind = CDKIND_ETN;
	else
	{
		bEachCall = TRUE;
		if (!_szType.CompareNoCase("FRMASTER"))				codeR.kind = CDKIND_FOREIGN_MAJOR;		// �ؿ��ֿ�����
		else if (!_szType.CompareNoCase("FRALLMASTER"))		codeR.kind = CDKIND_FOREIGN_ALL;		// �ؿ�������
		else if (!_szType.CompareNoCase("�ؿ�_����"))		codeR.kind = CDKIND_FOREIGN;			// �ٿ�30
		else if (!_szType.CompareNoCase("�ؿ�_����"))		codeR.kind = CDKIND_FOREIGN;			// ������100
		else if (!_szType.CompareNoCase("�ؿ�_����"))		codeR.kind = CDKIND_FOREIGN;			// S&P500
		else if (!_szType.CompareNoCase("�ؿ�_�ݸ�"))		codeR.kind = CDKIND_FOREIGN;			// CME����
		else if (!_szType.CompareNoCase("�ؿ�_��ä"))		codeR.kind = CDKIND_FOREIGN;			// ��Ÿ����
		else if (!_szType.CompareNoCase("�ؿ�_ȯ��"))		codeR.kind = CDKIND_FOREIGN;			// �����ݸ�
		else if (!_szType.CompareNoCase("FOFLMASTER"))		codeR.kind = CDKIND_FOINDEX_FO;			// FO��ǥ
		else if (!_szType.CompareNoCase("FOMASTER"))		codeR.kind = CDKIND_FOINDEX_MAJOR;		// FO �ֿ�����
		else if (!_szType.CompareNoCase("FOFIMASTER"))		codeR.kind = CDKIND_FOINDEX_FI;			// FO F.I����
		else if (!_szType.CompareNoCase("FOVOLMASTER"))		codeR.kind = CDKIND_FOINDEX_FUTURE;		// FO �����ܷ�
		else if (!_szType.CompareNoCase("FOMAKETMASTER"))	codeR.kind = CDKIND_FOINDEX_MARKET;		// FO ���嵿��
		else if (!_szType.CompareNoCase("��ǰ����"))		codeR.kind = CDKIND_KOFEX;				// ������ǰ����
		else if (!_szType.CompareNoCase("COMMODITYMASTER"))	codeR.kind = CDKIND_KOFEX_FUTURE;		// ������ǰ����
		else if (!_szType.CompareNoCase("�޷��ɼ�"))		codeR.kind = CDKIND_KOFEX_OPTION;		// �޷�(USD)�ɼ�
		else	bEachCall = FALSE;
	}

	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	if (bEachCall==FALSE)
	{
		if(	codeR.kind==CDKIND_KOSPI	||
			codeR.kind==CDKIND_KOSDAQ	||
			codeR.kind==CDKIND_KRX		||
			codeR.kind==CDKIND_KNX			)			
		{
			_DoParsingCode(strRet, arrCode, arrName, false, 1);
		}
		else if (codeR.kind==CDKIND_THEME)
		{
			if(strKey.IsEmpty())	//Sort��Ű��.
			{
				_DoParsingCode(strRet, arrCode, arrName);
				_DoSortCode(arrCode, arrName);
			}
			else
			{
				_DoParsingCode(strRet, arrCode, arrName, false, 1);
			}
		}
		else
		{
			_DoParsingCode(strRet, arrCode, arrName);
		}
	}
	else
	{
		if (_szType.CompareNoCase("PMASTER")==0)
			_DoParsingCodePMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("JPMASTER")==0)
			_DoParsingCodeJPMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("�޷��ɼ�")==0)
			_DoParsingCodeDPMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("MINI_PMASTER") == 0)
			_DoParsingCodeMiniMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("WEEKLY_PMASTER") == 0)
			_DoParsingCodeWeeklyMaster(strRet, arrCode, arrName);
		else	//	�ؿ�
			_DoParsingCode(strRet, arrCode, arrName);
			//_DoParsingCodeForeign(strRet, arrCode, arrName);
	}

	CString strTemp;
	if (arrCode.GetSize() > 0)
		strTemp = arrCode.GetAt(0);

	int nCount, i;
	if (codeR.kind == CDKIND_KOFEX)
	{
		nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strncmp(strTemp, g_pszCFTiesCode[i], 3) == 0)
			{
				strTemp = g_pszCFTiesCode[i];
				arrCode.Add(strTemp);

				strTemp.Format(_T("%s ���輱��"), g_pszCFTiesName[i]);
				arrName.Add(strTemp);

				break;
			}
		}
	}
	else if (codeR.kind == CDKIND_SFUTURE)
	{
		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strncmp(strTemp, g_pszSFTiesCode[i], 3) == 0)
			{
				strTemp = g_pszSFTiesCode[i];
				arrCode.Add(strTemp);

				strTemp.Format(_T("%s ���輱��"), g_pszSFTiesName[i]);
				arrName.Add(strTemp);
				break;
			}
		}
	}

	return TRUE;
}

BYTE CMasterDataManager::GetCodeTypeFromCode(LPCSTR strCode)
{
	BYTE rCodeType = 0xFF;
// 	if(strcmp(strCode, DEF_CONTINUE_FUT_CODE)==0)
// 	{
// 		rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
// 		return rCodeType;
// 	}
	if (strlen(strCode)==8 && strncmp(&strCode[3], "99999", 5)==0)
	{
		if(strcmp(strCode, DEF_CONTINUE_FUT_CODE)==0)		
		{
			rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
			return rCodeType;
		}
		else if( strcmp(strCode, DEF_CONTINUE_MINI_FUT_CODE) == 0)
		{
			rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
			return rCodeType;
		}

		int i, nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszCFTiesCode[i]) == 0)
				return CDRCodeTypeInfo::CODE_FUOPT_COMMODITY;
		}

		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszSFTiesCode[i]) == 0)
				return CDRCodeTypeInfo::CODE_STOCKFUT;
		}
	}
	
	rCodeType = _DoPreChkCodeSB(&m_RecentCodeTypeList, strCode);
	if(rCodeType != 0xFF )
		return rCodeType;



	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return CMasterTypeInfo::STOCK_CHART;

	if(strlen(strCode)==10)	//FO����
	{
		rCodeType = CDRCodeTypeInfo::CODE_FUOPT_JIPYO;
		_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
		return rCodeType;
	}

	rCodeType = 0xFF;
	//rCodeType = CDRCodeTypeInfo::CODE_KOSPI_STOCK;
	
	int rRet = (int)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);

// KHD : ���� ��ǰ ���� �� Spread Ÿ�� ���� 
	if(rRet < 0) 
	{   
		CString strItemCode = strCode;
		CString strTemp;
		strTemp = strItemCode.Left(1);
		if(strTemp == "4")
		{
			rRet = spreadType;
			strTemp = strItemCode.Mid(1, 2);
			if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
		}
		else if(strTemp == "1")
		{
			strTemp = strItemCode.Mid(1, 2);
			if( (atoi(strTemp) >= 10 && atoi(strTemp) <= 59)
				|| ( (strTemp[0] >= 'B' && strTemp[0] <= 'Z')	// �ֽļ��� �������� B0 ~ ZZ ����
					&& ((strTemp[1] >= '0' && strTemp[1] <= '9') || (strTemp[1] >= 'A' && strTemp[1] <= 'Z')) ) )
				rRet = sfutureType;
			else if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
		}
	}
// END
	switch(rRet)
	{
		case kospiType:
		case etfType:
		case elwType:
		case foreignType:	//20100930_JS.Kim �ܱ��� (ȭǳ����KDR(950010), �����ξ���Ƽ(950030))
		case etnType:
			{
				//return CDRCodeTypeInfo::CODE_KOSPI_STOCK;
				rCodeType = CDRCodeTypeInfo::CODE_KOSPI_STOCK;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case kosdaqType:
			{
				//return CDRCodeTypeInfo::CODE_KOSDAQ_STOCK;
				rCodeType = CDRCodeTypeInfo::CODE_KOSDAQ_STOCK;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case thirdType:
			{
				//return CDRCodeTypeInfo::CODE_ELW;
				rCodeType = CDRCodeTypeInfo::CODE_ELW;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case indexType:		// ����
			{
				//if(strCode[0]=='0') return CDRCodeTypeInfo::CODE_KOSPI_UPJONG;
				//return CDRCodeTypeInfo::CODE_KOSDAQ_UPJONG;
				if(strCode[0]=='0') rCodeType = CDRCodeTypeInfo::CODE_KOSPI_UPJONG;
				else rCodeType = CDRCodeTypeInfo::CODE_KOSDAQ_UPJONG;

				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}

		case futureType:	// ����
			{
				//return CDRCodeTypeInfo::CODE_KFUTURE;
				rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case spreadType:	// ��������
			{
				//return CDRCodeTypeInfo::CODE_FUTURE_SPREAD;
				rCodeType = CDRCodeTypeInfo::CODE_FUTURE_SPREAD;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case callType:		// �ݿɼ�
		case putType:		// ǲ�ɼ�
			{
				//return CDRCodeTypeInfo::CODE_KOPTION;
				rCodeType = CDRCodeTypeInfo::CODE_KOPTION;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}

		case sfutureType:	// �ֽļ���
			{
				//return CDRCodeTypeInfo::CODE_STOCKFUT;
				rCodeType = CDRCodeTypeInfo::CODE_STOCKFUT;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case koptionType:	// �ֽĿɼ�
			{
				//return CDRCodeTypeInfo::CODE_STOCKOPT;
				rCodeType = CDRCodeTypeInfo::CODE_STOCKOPT;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}

		//case foreignType:	// �ܱ���
		case usType:
			{
				//return CDRCodeTypeInfo::CODE_US_ITEM;
				rCodeType = CDRCodeTypeInfo::CODE_US_ITEM;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case cfutureType:
		{
				rCodeType = CDRCodeTypeInfo::CODE_FUOPT_COMMODITY;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rCodeType);
				return rCodeType;
		}
		//<<20100316_JS.Kim FX
// �ڼ��� >>
// 		case gfxType:
// 			{
// 				rCodeType = CDRCodeTypeInfo::CODE_FX;
// 				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
// 				return rCodeType;
// 			}
		//>>
// �ڼ��� <<

		default:
			//<<JS.Kim_20101019. �ű�ä �ڵ� �߰�
			//if (rRet >= THREE_BONDS && rRet <= MGOLD_FUTURE)
			if (rRet >= THREE_BONDS && rRet <= NEW_TEN_BONDS)
			//>>
			{
				rCodeType = CDRCodeTypeInfo::CODE_FUOPT_COMMODITY;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				break;
			}

			if(strstr(strCode, "@") != NULL || strstr(strCode, "$") != NULL)
				rCodeType = CDRCodeTypeInfo::CODE_US_ITEM;

			break;
	}

	return rCodeType;
}

BYTE CMasterDataManager::_DoPreChkCodeSB(List_ST_STRINGBYTE* pListST, LPCSTR strCode)
{
	ST_STRINGBYTE* pItem;
	
	for(POSITION pos=pListST->GetHeadPosition(); pos; )
	{
		pItem = pListST->GetNext(pos);
		if(pItem->m_szKey.Compare(strCode)==0)
			return pItem->m_Value;
	}
	return 0xFF;
}

void CMasterDataManager::_DoAddChkCodeSB(List_ST_STRINGBYTE* pListST, int nMaxCnt, LPCSTR strCode, BYTE bType)
{
	POSITION pos = pListST->GetHeadPosition();
	if(!pos)
	{
		// 		ST_STRINGBYTE* pItem = new ST_STRINGBYTE;
		// 		pItem->m_szKey = strCode;
		// 		pItem->m_Value = bType;
		// 		pListST->AddHead(pItem);
	}
	else
	{
		POSITION hPos=pListST->GetHeadPosition(),
			tPos;

		ST_STRINGBYTE* pItem;
		for(POSITION pos=hPos; pos; )
		{
			tPos = pos;
			pItem = pListST->GetNext(pos);
			if(pItem->m_szKey.Compare(strCode)==0)
			{
				if(tPos==hPos) return;
				else
				{
					pListST->RemoveAt(tPos);
					pListST->AddHead(pItem);
					return;
				}
			}
		}
		
		if(pListST->GetCount()==nMaxCnt)
			pListST->RemoveAt(tPos);
	}
	
	ST_STRINGBYTE* pItem = new ST_STRINGBYTE;
	pItem->m_szKey = strCode;
	pItem->m_Value = bType;
	pListST->AddHead(pItem);
}

BYTE CMasterDataManager::GetMarketTypeFromCode(LPCSTR strCode)
{
	BYTE rMarketType;
	if(strlen(strCode)==8 && strncmp(&strCode[3], "99999", 5)==0)
	{
		if(strcmp(strCode, DEF_CONTINUE_FUT_CODE)==0)		
		{
			rMarketType = CMasterTypeInfo::FUTOPT_CHART;
			return rMarketType;
		}
		else if( strcmp(strCode, DEF_CONTINUE_MINI_FUT_CODE) == 0)
		{
			rMarketType = CMasterTypeInfo::FUTOPT_CHART;
			return rMarketType;
		}

		int i, nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszCFTiesCode[i]) == 0)
				return CMasterTypeInfo::COMMODITY_CHART;
		}

		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszSFTiesCode[i]) == 0)
				return CMasterTypeInfo::STOCKFUTOPT_CHART;
		}
	}
	
	rMarketType = _DoPreChkCodeSB(&m_RecentMarketTypeList, strCode);
	if(rMarketType != 0xFF )
		return rMarketType;

	if(strstr(strCode, "@") != NULL || strstr(strCode, "$") != NULL)
	{
		//return CMasterTypeInfo::FOREIGN_CHART;
		rMarketType = CMasterTypeInfo::FOREIGN_CHART;
		_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
		return rMarketType;
	}

	if(strlen(strCode) == 0)//KHD ���°��
	{
		return CMasterTypeInfo::NONE_CHART;
	}
	if (strlen(strCode) <= 4)
	{
		//return CMasterTypeInfo::UPJONG_CHART;
		rMarketType = CMasterTypeInfo::UPJONG_CHART;
		_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
		return rMarketType;
	}
//@solomon	FO������ ���������� üũ�� �� �־� ����.
/*	if(strlen(strCode)==10)	//FO����
	{
		//return CMasterTypeInfo::FUOPT_JIPYO_CHART;
		rMarketType = CMasterTypeInfo::FUOPT_JIPYO_CHART;
		_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
		return rMarketType;
	}
*/
	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return CMasterTypeInfo::STOCK_CHART;

	CString strItemCode = strCode;

	int rRet = (int)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);
	if(rRet < 0)
	{
		CString strTemp;
		strTemp = strItemCode.Left(1);
		if(strTemp == "4")
		{
			rRet = spreadType;
			strTemp = strItemCode.Mid(1, 2);// �ֽļ����ϰ��� ��������� �ֽļ����� �����ش�.
			if( (atoi(strTemp) >= 10 && atoi(strTemp) <= 59)
				|| ( (strTemp[0] >= 'B' && strTemp[0] <= 'Z')	// �ֽļ��� �������� B0 ~ ZZ ����
					&& ((strTemp[1] >= '0' && strTemp[1] <= '9') || (strTemp[1] >= 'A' && strTemp[1] <= 'Z')) ) )
				rRet = sfutureType;
			else if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
		}
		else if(strTemp == "1")
		{
			strTemp = strItemCode.Mid(1, 2);
			if( (atoi(strTemp) >= 10 && atoi(strTemp) <= 59)
				|| ( (strTemp[0] >= 'B' && strTemp[0] <= 'Z')	// �ֽļ��� �������� B0 ~ ZZ ����
					&& ((strTemp[1] >= '0' && strTemp[1] <= '9') || (strTemp[1] >= 'A' && strTemp[1] <= 'Z')) ) )
				rRet = sfutureType;
			else if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
			else if (atoi(strTemp) == 1)
				rRet = futureType;
		}
		else if(strItemCode.Find("COMP")>= 0 || strItemCode.Find("SMBS")>= 0)
		{
			rRet = usType;
		}
	}

	switch(rRet)
	{
		case kospiType:
		case kosdaqType:
		case thirdType:
		case etfType:
		case elwType:
		case foreignType:	//20100930_JS.Kim �ܱ��� (ȭǳ����KDR(950010), �����ξ���Ƽ(950030))
		case etnType:
			{
				//return CMasterTypeInfo::STOCK_CHART;
				rMarketType = CMasterTypeInfo::STOCK_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		case indexType:		// ����
			{
				//return CMasterTypeInfo::UPJONG_CHART;
				rMarketType = CMasterTypeInfo::UPJONG_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		case futureType:	// ����
		case callType:		// �ݿɼ�
		case putType:		// ǲ�ɼ�
		case spreadType:	// ��������
			{
				//return CMasterTypeInfo::FUTOPT_CHART;
				rMarketType = CMasterTypeInfo::FUTOPT_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		case koptionType:	// �ֽĿɼ�
		case sfutureType:	// �ֽļ���
			{
				//return CMasterTypeInfo::STOCKFUTOPT_CHART;
				rMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		//case foreignType:	// �ܱ���
		case usType:
			{
				//return CMasterTypeInfo::FOREIGN_CHART;
				rMarketType = CMasterTypeInfo::FOREIGN_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}

		//<<20100511_JS.Kim �ؿܼ���
		case gfutureType:
			{
				rMarketType = CMasterTypeInfo::FO_FOREIGN_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		//>>

		//<<20100316_JS.Kim FX
// �ڼ��� >>
// 		case gfxType:
// 			{
// 				rMarketType = CMasterTypeInfo::FX_CHART;
// 				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
// 				return rMarketType;
// 			}
		//>>
// �ڼ��� <<
		case cfutureType:
			{
				rMarketType = CMasterTypeInfo::COMMODITY_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}

		default:
			if(strlen(strCode)==8)
			{
				if(strCode[0]=='2' || strCode[0]=='3')
				{
					rMarketType = CMasterTypeInfo::FUTOPT_CHART;
					return rMarketType;
				}
			}
			return CMasterTypeInfo::NONE_CHART;
			break;
	}

	return CMasterTypeInfo::STOCK_CHART;
}

// CString CMasterDataManager::GetJongmokNameFromCode(CString strCode)
// {
// 	return "";
// }

BOOL CMasterDataManager::GetRecentCodeNameArray(int nMarketType, CStringArray &arrCode, CStringArray &arrName)
{
	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return FALSE;

// 	LPCSTR aSymbol = "1301";
// 	switch(nMarketType)
// 	{
// 		case CMasterTypeInfo::STOCK_CHART: 
// 			break;
// 		case CMasterTypeInfo::UPJONG_CHART:			//UP
// 			aSymbol = "20301"; break;
// 		case CMasterTypeInfo::FUTOPT_CHART:			//����/�ɼ�
// 			//aSymbol = "focod"; break;
// 			aSymbol = "30301"; break;
// 		case CMasterTypeInfo::STOCKFUTOPT_CHART:	//�ֽļ���/�ֽĿɼ�
// 			aSymbol = "jfcod"; break;
// 		case CMasterTypeInfo::FOREIGN_CHART:		//�ؿ�
// 			aSymbol = "50301"; break;
// 		case CMasterTypeInfo::FUOPT_JIPYO_CHART:	//FO������ ���ǰ���.
// 			aSymbol = "60301"; break;
// 		default: return FALSE;
// 	}

	//DrCommon\PartnerMng\_PartnerUtilManager.cpp
	//CPartnerUtilManager::GetSymbolCode(..)��ƾ ������.
	CString szSymbol;
	switch(nMarketType)
	{
		case CMasterTypeInfo::STOCK_CHART:
			szSymbol = "1301";
			break;
		case CMasterTypeInfo::UPJONG_CHART:
			szSymbol = "20301";
			break;
		case CMasterTypeInfo::COMMODITY_CHART:
			//>>JS.Kim_20101025  ��ǰ���� �����丮 ��ȸ �ȵǾ� �����ڵ� ���� �� �ٽ� �ε� �ȵǴ� ���� ����
			//szSymbol = "cfcod ";
			szSymbol = "cfcod";
			//<<
			break;
		case CMasterTypeInfo::FUTOPT_CHART:
			szSymbol = "ed_focod";
			break;
		case CMasterTypeInfo::STOCKFUTOPT_CHART:
//			szSymbol = "sfcod";
			szSymbol = "50301";
			break;
		case CMasterTypeInfo::FOREIGN_CHART:
			szSymbol = "50301";
			break;
		case CMasterTypeInfo::FUOPT_JIPYO_CHART:	//091128���Ƿ� ������ ����.
			szSymbol = "109301";
			break;
		case CMasterTypeInfo::FO_FOREIGN_CHART:	//091128���Ƿ� ������ ����.
			szSymbol = "70301";
			break;
		default:
			return FALSE;
	}

	CString strRet;
	if (nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		CString strPath;
		strPath.Format(_T("%s\\sfcode_history.dat"), theApp.m_MainInfoMng.GetEncUserDir());
		CStdioFile file;
		if (file.Open(strPath, CFile::modeRead|CFile::typeText))
		{
			file.ReadString(strRet);
			file.Close();
		}
	}
	else
	{
		strRet = theApp._WinixVariant(hWinixWnd, historyCC, szSymbol);
	}

	_DoParsingCode2(strRet, arrCode, arrName);

	return TRUE;

//@Solomon090819-->
// 	LPIM_DATA *ppHistory= NULL;
// 	char eType;
// 	int nHisCnt = 0;
// 	__int64 nType64 = 0;
// 	CString str;
// 	nType64 |= __int64(1) << KSTOCK_KOSPI;
// 	nType64 |= __int64(1) << KSTOCK_KOSDAQ;
// 	nType64 |= __int64(1) << KSTOCK_SECTOR;
// 	nType64 |= __int64(1) << KSTOCK_ELW;
// 	nType64 |= __int64(1) << KFREEBOARD;
// 	nType64 |= __int64(1) << KKOSPI_UPJONG;
// 	nType64 |= __int64(1) << KKOSDAQ_UPJONG;
// 	nType64 |= __int64(1) << KSECTOR_UPJONG;
// 	nType64 |= __int64(1) << KFUTURES_MASTER;
// 	nType64 |= __int64(1) << KOPTIONS_MASTER;
// 	nType64 |= __int64(1) << KFUTURE_SBASE;
// 	nType64 |= __int64(1) << KOPTION_SBASE;
// 	nType64 |= __int64(1) << US_JISU;
// 	
// 	// MT_STOCK
// 	if(nMarketType == 0)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		IMD_KOSPI_STOCK* pKospiStock = NULL;
// 		IMD_KOSDAQ_STOCK* pKosdaqStock = NULL;
// 		IMD_ELW* pELW = NULL;
// 		IMD_FREEBOARD* pFreeboard = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_KOSPI_STOCK)
// 			{
// 				if(pKospiStock = (IMD_KOSPI_STOCK*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiStock->sCode), sizeof(pKospiStock->sCode), pKospiStock->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiStock->sName), sizeof(pKospiStock->sName), pKospiStock->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KOSDAQ_STOCK)
// 			{
// 				if(pKosdaqStock = (IMD_KOSDAQ_STOCK*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKosdaqStock->sCode), sizeof(pKosdaqStock->sCode), pKosdaqStock->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKosdaqStock->sName), sizeof(pKosdaqStock->sName), pKosdaqStock->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_ELW)
// 			{
// 				if(pELW = (IMD_ELW*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pELW->sCode), sizeof(pELW->sCode), pELW->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pELW->sName), sizeof(pELW->sName), pELW->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KFREEBOARD)
// 			{
// 				if(pFreeboard = (IMD_FREEBOARD*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pFreeboard->sCode), sizeof(pFreeboard->sCode), pFreeboard->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pFreeboard->sName), sizeof(pFreeboard->sName), pFreeboard->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 	
// 	// MT_UPJONG
// 	if(nMarketType == 1)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_KOSPI_UPJONG* pKospiUpjong = NULL;
// 		IMD_KOSDAQ_UPJONG* pKosdaqUpjong = NULL;
// 		IMD_SECTOR* pSector = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_KOSPI_UPJONG)
// 			{
// 				if(pKospiUpjong = (IMD_KOSPI_UPJONG*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiUpjong->sCode), sizeof(pKospiUpjong->sCode), pKospiUpjong->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiUpjong->sName), sizeof(pKospiUpjong->sName), pKospiUpjong->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KOSDAQ_UPJONG)
// 			{
// 				if(pKosdaqUpjong = (IMD_KOSDAQ_UPJONG*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKosdaqUpjong->sCode), sizeof(pKosdaqUpjong->sCode), pKosdaqUpjong->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKosdaqUpjong->sName), sizeof(pKosdaqUpjong->sName), pKosdaqUpjong->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_SECTOR)
// 			{
// 				if(pSector = (IMD_SECTOR*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pSector->sCode), sizeof(pSector->sCode), pSector->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pSector->sName), sizeof(pSector->sName), pSector->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}		
// 
// 
// 	// MT_FUTOPT
// 	if(nMarketType == 2)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_FUTURE* pFuture = NULL;
// 		IMD_OPTION* pOption = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_KFUTURE)
// 			{
// 				if(pFuture = (IMD_FUTURE*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pFuture->sCode), sizeof(pFuture->sCode), pFuture->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pFuture->sName), sizeof(pFuture->sName), pFuture->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KOPTION)
// 			{
// 				if(pOption = (IMD_OPTION*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pOption->sCode), sizeof(pOption->sCode), pOption->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pOption->sName), sizeof(pOption->sName), pOption->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 
// 	// MT_STOCKFUTOPT
// 	if(nMarketType == 3)	
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_KOSPI_SFUTURE* pKospiSFuture = NULL;
// 		IMD_KOSPI_SOPTION* pKospiSOption = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_FUTURE_SBASE)
// 			{
// 				if(pKospiSFuture = (IMD_KOSPI_SFUTURE*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiSFuture->sCode), sizeof(pKospiSFuture->sCode), pKospiSFuture->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiSFuture->sName), sizeof(pKospiSFuture->sName), pKospiSFuture->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_OPTION_SBASE)
// 			{
// 				if(pKospiSOption = (IMD_KOSPI_SOPTION*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiSOption->sCode), sizeof(pKospiSOption->sCode), pKospiSOption->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiSOption->sName), sizeof(pKospiSOption->sName), pKospiSOption->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 	
// 	// MT_FOREIGN
// 	if(nMarketType == 4)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_US_JISU* pUSJisu = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_US_JISU)
// 			{
// 				if(pUSJisu = (IMD_US_JISU*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pUSJisu->sCode), sizeof(pUSJisu->sCode), pUSJisu->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pUSJisu->sName), sizeof(pUSJisu->sName), pUSJisu->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 	
//@Solomon090819<--
	return TRUE;
}

void CMasterDataManager::_DoParsingCode(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert, int nStartIdx)
{
	if(szCodeList.GetLength()==0) return;

// 	szCodeList += "\n";
// 	int nPos1 = 0, nPos2, nOffset=0;
// 	CString szTmp, szCode, szCodeName;
// 
// 	int nDataLen = szCodeList.GetLength();
// 	if(nStartIdx==0)
// 	{
// 		while(nPos1>=0 && nOffset<=nDataLen)
// 		{
// 			nPos1 = szCodeList.Find('\n', nOffset);
// 			if(nPos1<0) break;
// 			
// 			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
// 			nOffset = (nPos1+1);
// 			
// 			nPos2 = szTmp.Find('\t');
// 			if(nPos2<0) break;
// 			szCode		= szTmp.Left(nPos2);
// 			if(szCode.Left(1) == "A") szCode = szCode.Mid(1);
// 			szCodeName	= szTmp.Mid(nPos2+1);
// 			
// 			szCodeName.TrimLeft();
// 			if(bInsert)
// 			{
// 				arrCode.InsertAt(0, szCode);
// 				arrName.InsertAt(0, szCodeName);
// 			}
// 			else
// 			{
// 				arrCode.Add(szCode);
// 				arrName.Add(szCodeName);
// 			}
// 		}
// 	}
// 	else if(nStartIdx==1)
// 	{
// 		while(nPos1>=0 && nOffset<=nDataLen)
// 		{
// 			nPos1 = szCodeList.Find('\n', nOffset);
// 			if(nPos1<0) break;
// 			
// 			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
// 			nOffset = (nPos1+1);
// 			
// 			nPos2 = szTmp.Find('\t');
// 			if(nPos2<0) break;
// 			szCode		= szTmp.Left(nPos2);
// 			if(szCode.Left(1) == "A") szCode = szCode.Mid(1);
// 			szCodeName	= szTmp.Mid(nPos2+1);
// //			szCodeName.TrimLeft();
// 			if(szCodeName[0]=='#' ||szCodeName[0]=='&')
// 				szCodeName = szCodeName.Mid(1);
// 
// 			if(bInsert)
// 			{
// 				arrCode.InsertAt(0, szCode);
// 				arrName.InsertAt(0, szCodeName);
// 			}
// 			else
// 			{
// 				arrCode.Add(szCode);
// 				arrName.Add(szCodeName);
// 			}
// 		}
// 	}

	CString strData, strTemp, strCode, strName;
	int nPos;
	strData = szCodeList;
	
	while(TRUE)
	{
		nPos = strData.Find("\t");
		if(nPos == -1)
			break;
		
		strTemp = strData.Mid(0, nPos + 1);
		strTemp.Remove('\t');
		strData = strData.Mid(nPos + 1, strData.GetLength());
		strCode.Format("%s", strTemp);

		nPos = strData.Find("\t");

		strTemp = strData.Mid(0, nPos + 1);
		strTemp.Remove('\t');
		strData = strData.Mid(nPos + 1, strData.GetLength());
		strName.Format("%s", strTemp);

		if (strCode.GetLength() <= 0 || strName.GetLength() <= 0)
			continue;

		arrCode.Add(strCode);
		arrName.Add(strName);
	}
}

void CMasterDataManager::_DoParsingCodePMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
// 	szCodeList += "\n";
// 	int nPos1 = 0, nPos2, nOffset=0;
// 	CString szTmp, szCode, szCodeName;
// 	const int nRequest = 3;
// 	CString szTmpA[nRequest], szMonth;
// 	char cCallPut;
// 
// 	const int nMaxOption = 4;
// 	int	nOptIndex=-1;
// 	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
// 
// 	int nDataLen = szCodeList.GetLength();
// 	while(nPos1>=0 && nOffset<=nDataLen)
// 	{
// 		nPos1 = szCodeList.Find('\n', nOffset);
// 		if(nPos1<0) break;
// 		
// 		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
// 		nOffset = (nPos1+1);
// 		
// 		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
// 		szCode	= szTmp.Left(nPos2);
// 		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
// 		
// 		for(int i=0; i<nRequest-1; i++)
// 		{
// 			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
// 			szTmpA[i] = szTmp.Left(nPos2);
// 			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
// 		}
// 		
// 		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
// 		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
// 		
// 		szTmpA[1] = szTmp.Mid(nPos2+1);
// 		
// 		cCallPut = szTmpA[0].GetAt(0);
// 		if(szTmpA[0]=="��") cCallPut = 'C';
// 		else if(szTmpA[0]=="ǲ") cCallPut = 'P';
// 
// 		if(szMonth.Compare(szTmpA[1]) !=0 )
// 		{
// 			szMonth = szTmpA[1];
// 			nOptIndex ++;
// 			if(nOptIndex>=nMaxOption) nOptIndex=0;
// 		}
// 
// 		szCodeName.Format("%c %-6.6s %-10.10s", cCallPut, szTmpA[1], szTmpA[2]);
// 
// 		//arrCode.InsertAt(0, szCode);
// 		//arrName.InsertAt(0, szCodeName);
// 		tmpArrCode[nOptIndex].InsertAt(0, szCode);
// 		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
// 	}
// 
// 	//for(int k=nMaxOption-1; k>=0; k--)
// 	for(int k=0; k<nMaxOption; k++)
// 	{
// 		int nCnt = tmpArrName[k].GetSize();
// 		for(int j=0; j<nCnt; j++)
// 		{
// 			arrCode.Add(tmpArrCode[k].GetAt(j));
// 			arrName.Add(tmpArrName[k].GetAt(j));
// 		}
// 	}

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 3;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 11;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
		
		szCodeList = szTmp.Mid(nPos2+1);
		szCodeList += "\n";
		nOffset = 0;
		
		cCallPut = szTmpA[0].GetAt(0);
		if(szTmpA[0]=="��") cCallPut = 'C';
		else if(szTmpA[0]=="ǲ") cCallPut = 'P';
		
		if(szMonth.Compare(szTmpA[1]) !=0 )
		{
			szMonth = szTmpA[1];
			nOptIndex ++;
			if(nOptIndex>=nMaxOption) nOptIndex=0;
		}
		
		szCodeName.Format("%c %-6.6s %-10.10s", cCallPut, szTmpA[1], szTmpA[2]);
		szCodeName.TrimRight();
		
		//arrCode.InsertAt(0, szCode);
		//arrName.InsertAt(0, szCodeName);
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}
	
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}

//@Solomon:�ֽĿɼǿ�.
void CMasterDataManager::_DoParsingCodeJPMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 4;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;

	const int nMaxOption = 4;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];

	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();

		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}

		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);

		szTmpA[2] = szTmp.Mid(nPos2+1);

		cCallPut = szTmpA[1].GetAt(0);
		if(szTmpA[1]=="��") cCallPut = 'C';
		else if(szTmpA[1]=="ǲ") cCallPut = 'P';

		if(szMonth.Compare(szTmpA[2]) !=0 )
		{
			szMonth = szTmpA[2];
			nOptIndex ++;
		}

		szCodeName.Format("%11s%c 20%-4.4s %-10.10s", " ", cCallPut, szTmpA[2], szTmpA[3]);

		//arrCode.InsertAt(0, szCode);
		//arrName.InsertAt(0, szCodeName);
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}

	//for(int k=nMaxOption-1; k>=0; k--)
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}


//@Solomon:�޷��ɼǿ�.
void CMasterDataManager::_DoParsingCodeDPMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 4;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 4;
	int	nOptIndex=0, nIndex=0;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	CMap<CString, LPCTSTR, int, int> mapMonth;

	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		szTmpA[nRequest-1] = szTmp;

		if(szTmpA[1]=="��") cCallPut = 'C';
		else if(szTmpA[1]=="ǲ") cCallPut = 'P';
		
		if (!mapMonth.Lookup(szTmpA[2], nIndex))
		{
			mapMonth.SetAt(szTmpA[2], nOptIndex);
			nIndex = nOptIndex;
			nOptIndex++;
		}

		szCodeName.Format("%11s%c %-6.6s %-10.10s", " ", cCallPut, szTmpA[2], szTmpA[3]);
		
		//arrCode.InsertAt(0, szCode);
		//arrName.InsertAt(0, szCodeName);
		if (nIndex >= nMaxOption)
		{
			break;
		}

		tmpArrCode[nIndex].InsertAt(0, szCode);
		tmpArrName[nIndex].InsertAt(0, szCodeName);
	}
	
	//for(int k=nMaxOption-1; k>=0; k--)
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}

void CMasterDataManager::_DoParsingCodeForeign(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');
		if(nPos2<0) break;
		szCode		= szTmp.Left(nPos2);
		szCodeName	= szTmp.Mid(nPos2+1);
		
		arrCode.Add(szCode);
		szCodeName.TrimLeft();
		arrName.Add(szCodeName);
	}
}

void CMasterDataManager::_DoParsingCode2(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\t', nOffset);
		if(nPos1<0) break;

		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find(' ');
		if(nPos2<0) break;

		szCode		= szTmp.Left(nPos2);
		szCodeName	= szTmp.Mid(nPos2+1);

		arrCode.Add(szCode);
		szCodeName.TrimLeft();
		arrName.Add(szCodeName);
	}
}

void CMasterDataManager::_DoParsingCodeFX(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert, int nStartIdx)
{
	if(szCodeList.GetLength()==0) return;

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;

	int nDataLen = szCodeList.GetLength();
	if(nStartIdx==0)
	{
		while(nPos1>=0 && nOffset<=nDataLen)
		{
			nPos1 = szCodeList.Find('\n', nOffset);
			if(nPos1<0) break;
			
			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
			nOffset = (nPos1+1);
			
			nPos2 = szTmp.Find('\t');
			if(nPos2<0) break;
			szCode		= szTmp.Left(nPos2);
			szCodeName	= szTmp.Mid(nPos2+1);
			
			szCodeName.TrimLeft();
			if(bInsert)
			{
				arrCode.InsertAt(0, szCode);
				arrName.InsertAt(0, szCodeName);
			}
			else
			{
				arrCode.Add(szCode);
				arrName.Add(szCodeName);
			}
		}
	}
	else if(nStartIdx==1)
	{
		while(nPos1>=0 && nOffset<=nDataLen)
		{
			nPos1 = szCodeList.Find('\n', nOffset);
			if(nPos1<0) break;
			
			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
			nOffset = (nPos1+1);
			
			nPos2 = szTmp.Find('\t');
			if(nPos2<0) break;
			szCode		= szTmp.Left(nPos2);
			szCodeName	= szTmp.Mid(nPos2+1);
			if(szCodeName[0]=='#' ||szCodeName[0]=='&')
				szCodeName = szCodeName.Mid(1);

			if(bInsert)
			{
				arrCode.InsertAt(0, szCode);
				arrName.InsertAt(0, szCodeName);
			}
			else
			{
				arrCode.Add(szCode);
				arrName.Add(szCodeName);
			}
		}
	}
}

BOOL CMasterDataManager::GetAllStock(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ALL;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, arrCode, arrName);

	return TRUE;
}

BOOL CMasterDataManager::GetStockCodeWithType(int nType, CStringArray &arrCode, CStringArray &arrName)
{
	if(nType=='0')
	{
		GetMasterDataInUpDetails("UMaster", arrCode, arrName, "1001'");
	}
	else if(nType=='1')
	{
		GetMasterDataInUpDetails("kumASTER", arrCode, arrName, "2001'");
	}

	//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(nType, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 	
// 	CString str;
// 	IMD_KOSPI_STOCK* pKospiStock = NULL;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pKospiStock = (IMD_KOSPI_STOCK*)(ppData[nIndx]->pData))
// 		{
// 			str = ppData[nIndx]->szCode; str.TrimRight();
// 			arrCode.Add(str);
// 			str = ppData[nIndx]->szName; str.TrimRight();
// 			arrName.Add(str);
// 		}
// 	}
	//@Solomon090819<--

	return TRUE;
}

/*
BOOL CMasterDataManager::GetFOFLMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_FOINDEX_FO_MONTH;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, arrCode, arrName);

	return TRUE;
}
*/

BOOL CMasterDataManager::GetJFMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_SFUTURE;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName);
	
//@Solomon090819
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_FUTURE_SBASE, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	LPIMD_KOSPI_SFUTURE pSFut = NULL;
// 	CMapStringToString mapName;
// 	CString str;
// 	CString strComp;
// 
// 	char* szName = new char[sizeof(pSFut->sunas_name) + 1];
// 
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pSFut = (LPIMD_KOSPI_SFUTURE)(ppData[nIndx]->pData))
// 		{
// 			::memset(szName, 0x00, sizeof(pSFut->sunas_name) + 1);
// 			::memcpy(szName, pSFut->sunas_name, sizeof(pSFut->sunas_name));
// 
// 			str = szName;
// 			str.TrimRight();
// 			if(mapName.Lookup(str, strComp) == FALSE)
// 			{
// 				mapName.SetAt(str, str);
// 				arrCode.Add(str);
// 				arrName.Add(str);
// 			}
// 		}
// 	}
// 
// 	delete [] szName;
// 	mapName.RemoveAll();
	//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetJPMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_SOPTION;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName, true);

	return TRUE;
}

// 	BOOL CMasterDataManager::GetFreeBoardMst(CStringArray &arrCode, CStringArray &arrName)
// 	{
// 		arrCode.RemoveAll();
// 		arrName.RemoveAll();
// 
// 	//@Solomon090819-->
// 	// 	int nCount = 0;
// 	// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_FREEBOARD_UPJONG, &nCount);
// 	// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 	// 
// 	// 	CString str;
// 	// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	// 	{
// 	// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 	// 		arrCode.Add(str);
// 	// 		str = ppData[nIndx]->szName; str.TrimRight();
// 	// 		arrName.Add(str);
// 	// 	}
// 	//@Solomon090819<--
// 
// 		return TRUE;
// 	}

BOOL CMasterDataManager::GetELWIndexMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ELW_BASE;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_ELW, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	IMD_ELW* pElw = NULL;
// 	CMapStringToString mapName;
// 	CString str;
// 	CString strComp;
// 
// 	char* szName = new char[sizeof(pElw->sBaseCode1) + 1];
// 
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pElw = (IMD_ELW*)(ppData[nIndx]->pData))
// 		{
// 			::memset(szName, 0x00, sizeof(pElw->sBaseCode1) + 1);
// 			::memcpy(szName, pElw->sBaseCode1, sizeof(pElw->sBaseCode1));
// 
// 			str = m_itemMaster.GetItemName(IMT_STOCK, szName);
// 			if(str.IsEmpty())
// 				str = szName;
// 			str.TrimRight();
// 			if(mapName.Lookup(str, strComp) == FALSE)
// 			{
// 				mapName.SetAt(str, str);
// 				arrCode.Add(str);
// 				arrName.Add(str);
// 			}
// 		}
// 	}
// 
// 	delete [] szName;
// 	mapName.RemoveAll();
//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetELWIndexCode(CString strELWCode, CString& strIndexCode, CString& strIndexName)
{
	strIndexCode.Empty();
	strIndexName.Empty();

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_ELW, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	IMD_ELW* pElw = NULL;
// 
// 	char* szCode = new char[sizeof(pElw->sCode) + 1];
// 
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pElw = (IMD_ELW*)(ppData[nIndx]->pData))
// 		{
// 			::memset(szCode, 0x00, sizeof(pElw->sCode) + 1);
// 			::memcpy(szCode, pElw->sCode, sizeof(pElw->sCode));
// 
// 			str = szCode;
// 			str.TrimRight();
// 
// 			if(str == strELWCode)
// 			{
// 				char* szIndexCode = new char[sizeof(pElw->sBaseCode1) + 1];
// 				::memset(szIndexCode, 0x00, sizeof(pElw->sBaseCode1) + 1);
// 				::memcpy(szIndexCode, pElw->sBaseCode1, sizeof(pElw->sBaseCode1));
// 
// 				strIndexCode = szIndexCode;
// 				strIndexCode.TrimRight();
// 				delete [] szIndexCode;
// 
// 				if(strIndexCode == "KSP200")
// 				{
// 					strIndexCode = "101";
// 					strIndexName = m_itemMaster.GetItemName(IMT_KOSPI200_UPJONG, strIndexCode);
// 				}
// 				else
// 					strIndexName = m_itemMaster.GetItemName(IMT_STOCK, strIndexCode);
// 
// 				break;
// 			}
// 		}
// 	}
// 	delete [] szCode;

//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetELWIssueMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ELW_PB;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_ELW, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	IMD_ELW* pElw = NULL;
// 	CMapStringToString mapName;
// 	CString str;
// 	CString strComp;
// 	char* szName = new char[sizeof(pElw->sPublishName) + 1];
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pElw = (IMD_ELW*)(ppData[nIndx]->pData))
// 		{
// 			::memset(szName, 0x00, sizeof(pElw->sPublishName) + 1);
// 			::memcpy(szName, pElw->sPublishName, sizeof(pElw->sPublishName));
// 
// 			str = szName;
// 			str.TrimRight();
// 			if(mapName.Lookup(str, strComp) == FALSE)
// 			{
// 				mapName.SetAt(str, str);
// 				arrCode.Add(str);
// 				arrName.Add(str);
// 			}
// 		}
// 	}
// 	delete [] szName;
// 	mapName.RemoveAll();
//@Solomon090819<--
	return TRUE;
}

CString CMasterDataManager::GetCodeName(LPCTSTR lpszItemCode)
{
//	if(strcmp(lpszItemCode, DEF_CONTINUE_FUT_CODE)==0) 
//		return SZDEF_CONTINUE_FUT_CODE;

	if(strlen(lpszItemCode)==8 && strncmp(&lpszItemCode[3], "99999", 5)==0)
	{
		if (strcmp(lpszItemCode, DEF_CONTINUE_FUT_CODE)==0)
			return SZDEF_CONTINUE_FUT_CODE;
		else if( strcmp(lpszItemCode, DEF_CONTINUE_MINI_FUT_CODE) == 0 )
			return SZDEF_CONTINUE_MINI_FUT_CODE;

		CString strTemp;
		int i, nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(lpszItemCode, g_pszCFTiesCode[i]) == 0)
			{
				strTemp.Format(_T("%s ���輱��"), g_pszCFTiesName[i]);
				return strTemp;
			}
		}

		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(lpszItemCode, g_pszSFTiesCode[i]) == 0)
			{
				strTemp.Format(_T("%s ���輱��"), g_pszSFTiesName[i]);
				return strTemp;
			}
		}
	}

	CString szItemCode = lpszItemCode; //winix�������� �����Ͱ� ����Ǳ⶧���� �̷��� ��.
	// �̻�� (20151123) : (ISSUE NO - 0013117): [7125] �ֱ��ư Ŭ���Ͽ� �ֱ� �����ϴٺ��� ��ȸ�����ʰ� HTS ���������� �Ǵ� ����
	//			winix ���װ� �ƴϰ�, �����͸� �Ѱ��ָ�.. �Լ������� �Ǵ� �Ѱ��� ������ ���� ����Կ� �־� ������ �ɼ� ����.
	//			�Ͽ�, (ISSUE NO - 0013117)�� _DoAddChkCodeSS() �Լ� ������ �����ϴ� ���� �����ͷ� ����Ǿ� �߻��ϴ� ����.
	szItemCode.TrimLeft();szItemCode.TrimRight();

	//20221210 �̹��� - �����ڵ尡 ���ڿ��� ��� hWinixWnd�ڵ鿡 WM_USER�޼����� SendMessage�� ���޽� �޸� �����鼭 CStdDialog::m_strRecvCode�� �����Ⱚ ���� ��ȸ����ó���� �����߻� >>
	if (szItemCode.IsEmpty())
		return "";
	//20221210 �̹��� - �����ڵ尡 ���ڿ��� ��� hWinixWnd�ڵ鿡 WM_USER�޼����� SendMessage�� ���޽� �޸� �����鼭 CStdDialog::m_strRecvCode�� �����Ⱚ ���� ��ȸ����ó���� �����߻� <<

	CString strRet;
	BOOL bFind = _DoPreChkCodeSS(&m_RecentCodeNameList, (LPTSTR)(LPCTSTR)szItemCode, strRet);
	if(bFind) return strRet;

	//FO���� CodeName.
// 	if(strlen(lpszItemCode)==10)
// 	{
// 		CString rValue;
// 		gFOCodeMap.Lookup(lpszItemCode, rValue);
// 		return rValue;
// 	}

	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return "";

	strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(nameDLL, codeNAME), (LPARAM)(LPCTSTR)szItemCode);
	if(strRet.IsEmpty())
	{
		HWND hMasterWnd = GetCodeMasterHWND();
		if(!hMasterWnd) return "";
		szItemCode = lpszItemCode;
		strRet = (char*)::SendMessage(hMasterWnd, WM_USER, MAKEWPARAM(codeNAME, NULL), (LPARAM)(LPCTSTR)szItemCode);
	}
	strRet.TrimLeft(); 
	strRet.TrimRight();

/*@solom	20100122	ù��° ���� Ư���뵵 ������� ����.
	BYTE rCodeType = GetCodeTypeFromCode(lpszItemCode);
	if( rCodeType==CDRCodeTypeInfo::CODE_KOSPI_STOCK ||
		rCodeType==CDRCodeTypeInfo::CODE_KOSDAQ_STOCK ||
		rCodeType==CDRCodeTypeInfo::CODE_ELW)
//	if(strRet[0]=='&')
		strRet = strRet.Mid(1); //ù��°�� space�Ǵ� &���� ���ڰ� ���� �����Ƿ�. �� ĭ �ٿ�.
*/
	strRet.Replace("&", "&&");

	if(strRet.GetLength())	_DoAddChkCodeSS(&m_RecentCodeNameList, RecentGetCodeName_Max, (LPTSTR)(LPCTSTR)szItemCode, strRet);

	return strRet;
}

CString CMasterDataManager::GetItemExpCode(LPCTSTR lpszCode)
{
//@Solomon090819	char cMarketType = m_itemMaster.GetItemType(lpszCode);
//@Solomon090819	return m_itemMaster.GetItemExpCode(cMarketType, lpszCode);
	return "";//@Solomon090819
}

BOOL CMasterDataManager::GetThemeGroup(CStringArray& arCode, CStringArray& arName)
{
	CString strCode;
	strCode.Format("%d", CDKIND_THEMELIST);
	GetMasterDataInUpDetails("TMMASTER", arCode, arName, strCode);
//@Solomon090819	int nResult = m_itemMaster.GetThemeGroup(arCode, arName);
//@Solomon090819	return nResult;
	return FALSE; //@Solomon090819
}

BOOL CMasterDataManager::GetThemeItemList(LPCTSTR szThemeCode, CStringArray &arrCode, CStringArray &arrName)
{
	GetMasterDataInUpDetails("TMMASTER", arrCode, arrName, szThemeCode);

//@Solomon090819	int nResult = m_itemMaster.GetThemeItemList2(szThemeCode, arrCode, arrName);	
//@Solomon090819	return nResult;
	return FALSE; //@Solomon090819
}

BOOL CMasterDataManager::_DoPreChkCodeSS(List_ST_STRINGSTRING* pListST, LPCSTR strCode, CString &szCodeName)
{
	if(!pListST) return FALSE;

	ST_STRINGSTRING* pItem;	
	for(POSITION pos=pListST->GetHeadPosition(); pos; )
	{
		pItem = pListST->GetNext(pos);
		if(pItem->m_szKey.Compare(strCode)==0)
		{
			szCodeName = pItem->m_szValue;
			return TRUE;
		}
	}
	return FALSE;
}

void CMasterDataManager::_DoAddChkCodeSS(List_ST_STRINGSTRING* pListST, int nMaxCnt, LPCSTR strCode, LPCSTR szCodeName)
{
	if(strCode==NULL || strlen(strCode)==0) return;

	POSITION pos = pListST->GetHeadPosition();
	if(pos)
	{
		POSITION hPos=pListST->GetHeadPosition(),
			tPos;
		
		ST_STRINGSTRING* pItem;
		for(POSITION pos=hPos; pos; )
		{
			tPos = pos;
			pItem = pListST->GetNext(pos);
			if(pItem->m_szKey.Compare(strCode)==0)
			{
				if(tPos==hPos) return;
				else
				{
					pListST->RemoveAt(tPos);
					pListST->AddHead(pItem);
					return;
				}
			}
		}
		
		if(pListST->GetCount()==nMaxCnt)
			pListST->RemoveAt(tPos);
	}
	
	ST_STRINGSTRING* pItem = new ST_STRINGSTRING;
	pItem->m_szKey = strCode;
	pItem->m_szValue = szCodeName;
	pListST->AddHead(pItem);
}

BOOL CMasterDataManager::GetSiteUpKode(LPCSTR szCode, CString& rCode)
{
	rCode.Empty();

	BOOL bFind = _DoPreChkCodeSS(&m_RecentChxUpCodeList, szCode, rCode);
	if(bFind) return bFind;

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
//	szCode ="001";
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_CODECHANGE;
	codeR.type = CDTYPE_CODE;
	memcpy(codeR.code, szCode, strlen(szCode));
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	if(rCode.IsEmpty())
	{
		rCode = strRet;
		_DoAddChkCodeSS(&m_RecentChxUpCodeList, RecentGetChxUpCode_Max, szCode, rCode);
		return TRUE;
	}

	return FALSE;
}

void CMasterDataManager::_DoClearList()
{
	for(POSITION pos=m_UpKeyList.GetHeadPosition(); pos; )
		delete m_UpKeyList.GetNext(pos);
	m_UpKeyList.RemoveAll();
}

int gFncomparisonFunctionString(const void *a, const void *b) 
{
	return( strcmp( (char *)a, (char *)b) );
}

void CMasterDataManager::_DoSortCode(CStringArray &arrCode, CStringArray &arrName)
{
	int nCount = arrCode.GetSize();
	int nOneSize = 100;
	char* pData = new char[nOneSize*nCount];
	ZeroMemory(pData, nOneSize*nCount);
	char aTmp[100+1]={0,};

	//char** pArray = (char**)pData;

	int nStrLen;
	int i = 0;
	for( i=0; i<nCount; i++)
	{
		sprintf(aTmp, "%s%s", arrName.GetAt(i), arrCode.GetAt(i));

		nStrLen = strlen(aTmp);
		memcpy(&pData[i*nOneSize], aTmp, nStrLen);
		pData[i*nOneSize+nStrLen] = 0;
	}

	//qsort((void *)pArray, nCount, sizeof(pArray[0]), gFncomparisonFunctionString);
	qsort((void *)pData, nCount, nOneSize, gFncomparisonFunctionString);

	arrCode.RemoveAll();
	arrName.RemoveAll();
	CString szCode, szName;
	char* pOneData;
	for(i=0; i<nCount; i++)
	{
		pOneData = &pData[i*nOneSize];
		//szName.Format("%*.*s", nOneSize, nOneSize, pOneData);
		szName = pOneData;
		szCode = szName.Right(3);
		szName = szName.Left(szName.GetLength()-3);

		arrCode.Add(szCode);
		arrName.Add(szName);
	}
}

BOOL CMasterDataManager::_DoGetOldFutureMaster(CStringArray &arrCode, CStringArray &arrName)
{
	char *arrayCode[] = { "101DC000"	,"101E3000"	,"101E6000"	,"101E9000"	,NULL};
	char *arrayName[] = { "F 0912"		,"F 1003"	,"F 1006"	,"F 1009"	,NULL};

	int i=0;
	while(arrayCode[i] != NULL)
	{
		arrCode.Add(arrayCode[i]);
		arrName.Add(arrayName[i]);
		i++;
	}
	
	return TRUE;
}

BOOL CMasterDataManager::GetETNMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();
	
	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ETN;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	
	return TRUE;
}

BOOL CMasterDataManager::GetVFutureMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{	
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();
	
	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_VF;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	return TRUE;
}

BOOL CMasterDataManager::GetSFutureMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	return TRUE;
}

BOOL CMasterDataManager::GetMiniFutureJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName, CStringArray *&pastrJongEngName)
{
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();
	
	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_MINI_FUTURE;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrJongCode, *pastrJongName);

	if( pastrJongCode->GetSize() > 0 )
	{
		pastrJongCode->Add(DEF_CONTINUE_MINI_FUT_CODE);
		pastrJongName->Add(SZDEF_CONTINUE_MINI_FUT_CODE);
	}
	
	return TRUE;
}

BOOL CMasterDataManager::GetMiniOptionJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName)
{
	return GetMasterDataInUpDetails("MINI_PMASTER", *pastrJongCode, *pastrJongName, "");
}

BOOL CMasterDataManager::GetWeeklyOptionJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName)
{
	return GetMasterDataInUpDetails("WEEKLY_PMASTER", *pastrJongCode, *pastrJongName, "");
}

void CMasterDataManager::_DoParsingCodeMiniMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 3;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 11;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			
			int nTemp = 2;
			if( strncmp( szTmp , "M ", nTemp ) == 0 )			// ��簡�� 'M'(Mini)�� �߰��Ǿ� 'M'�� �ڸ��� ó��
				szTmp = szTmp.Mid(nTemp);
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
		
		szCodeList = szTmp.Mid(nPos2+1);
		szCodeList += "\n";
		nOffset = 0;
		
		cCallPut = szTmpA[0].GetAt(0);		
		if(szMonth.Compare(szTmpA[1]) !=0 )
		{
			szMonth = szTmpA[1];
			nOptIndex ++;
			if(nOptIndex>=nMaxOption) nOptIndex=0;
		}
		
		szCodeName.Format("%c %-7.7s %-9.9s", cCallPut, szTmpA[1], szTmpA[2]);
		szCodeName.TrimRight();
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}
	
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}


void CMasterDataManager::_DoParsingCodeWeeklyMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 3;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 11;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			
			int nTemp = 2;
			if( strncmp( szTmp , "M ", nTemp ) == 0 )			// ��簡�� 'M'(Mini)�� �߰��Ǿ� 'M'�� �ڸ��� ó��
				szTmp = szTmp.Mid(nTemp);
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
		
		szCodeList = szTmp.Mid(nPos2+1);
		szCodeList += "\n";
		nOffset = 0;
		
		cCallPut = szTmpA[0].GetAt(0);		
		if(szMonth.Compare(szTmpA[1]) !=0 )
		{
			szMonth = szTmpA[1];
			nOptIndex ++;
			if(nOptIndex>=nMaxOption) nOptIndex=0;
		}
		
		szCodeName.Format("%c %-7.7s %-9.9s", cCallPut, szTmpA[1], szTmpA[2]);
		szCodeName.TrimRight();
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}
	
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}

