#if !defined(AFX_INTERFACE_OF_MASTERDATAMANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_MASTERDATAMANAGER_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IMasterDataMng.h"
// 

#include "IBaseDefine.h"
#include "./MasterDefine.h"
#include "./MasterDataSub.h"

//#define		UUID_IMasterDataManager				5
//#define		UUID_IMasterDataManager0712			UUID_IMasterDataManager
//#define		UUID_IMasterDataManager080331		UUID_IMasterDataManager
//#define		UUID_IMasterDataManager099999		UUID_IMasterDataManager
//#define		UUID_IMasterDataManager080902		UUID_IMasterDataManager
#define		IMasterDataManagerLast				IMasterDataManager080902
/* -------------------------------------------------------------------------
���� : ������������ �ڵ鸵
���ù��� : ����_����������.doc
������  : ���ؿ� 2005.11.25(��)
History : 2005.11.25 �������̽� ����
----------------------------------------------------------------------------
- IMasterDataManager�������̽��� ����� �����ϱ� ���ؼ���
  ���ο��� CodeOcx�� IMainExInfoManager�� ���� ��������� �Ѵ�.

int nID = 5;
IMasterDataManager* pManager = (IMasterDataManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
IMasterDataManager* pManager = (IMasterDataManager*)AfxGetPctrInterface(nID);

ex1>
	CList<CJMaster, CJMaster>* list = 
		(CList<CJMaster, CJMaster>*)pManager->GetCodePointer("JMASTER"); 

ex2>
	int nID = 5;
	IMasterDataManager* pManager = (IMasterDataManager*)AfxGetPctrInterface(nID);
	if(pManager==NULL)
	{
		AfxMessageBox("IMasterDataManager Not Loaded");
		return;
	}
	int nTotal = pManager->LoadMASTER_J_K();
	CMaster_J_K* pData = pManager->J_K_GetMaster();
	for(int i=0; i<nTotal && i<3; i++)
	{
		CString s;
		s.Format("Code[%s], Name[%s]\n", pData[i].m_szCode, pData[i].m_szName);
		AfxMessageBox(s);
	}
	pManager->UnLoadMASTER_J_K();

ex3>
	...���⿡..

----------------------------------------------------------------------------
������������ $DATA/Master���丮�� ��ġ��.
����(205.11.25��)�� GetCodeList()�Լ��� ������.
// {{ Code.ocx �� // }} Code.ocx ���� �����ִ� �κ��� ������ code.ocx������ 
�״�� ���ϴ� �κ����� �Ķ���ͳ� ��ɿ� ���ؼ��� code.ocx�����ڿ��� ���ǹٶ�.
------------------------------------------------------------------------- */

DECLARE_INTERFACE(IMasterDataManagerBase)
{
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, LPCSTR szData) PURE;
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, long   dwData) PURE;
};

DECLARE_INTERFACE_(IMasterDataManager, IMasterDataManagerBase)
{
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// ������Ͱ� Code.ocx�� ���õ� �κ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// {{ Code.ocx
	/* --------------------------------------------------- 
	-. ����[GetBaseCodeTable]
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, GetBaseCodeTable)() PURE;

	/* --------------------------------------------------- 
	-. ����[SetInfo] codeocx ���������ε� �ϴ� ��� ����.
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, SetInfo)(long dwInfo) PURE;

	/* --------------------------------------------------- 
	-. ����[ShowCodeTable]
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	/* --------------------------------------------------- 
	-. ����[ShowModeCodeTable]���������ε� �ϴ� ��� ����.
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowModeCodeTable)(LPCTSTR szCodeKey, const VARIANT& hWnd) PURE;

	/* --------------------------------------------------- 
	-. ����[SetDataString]
	-. I-PARAM : nKey	10 : RootPath����, szData�� RootPath.
						20 : �����͸� �е��� ��û, szData������.
						30 : 6�ڸ��ڵ� 0 �ڵ� ��� ����, 1 = �ڵ����, 0 = �Ϲ�
		�Է��Ķ���Ϳ� ���� �ڼ��� ������ code.ocx�����ڿ��� ���ǹٶ�.
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, SetDataString)(short nKey, LPCTSTR szData) PURE;

	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(BOOL, InitData)(LPCTSTR szRootPath) PURE;
	
	/* --------------------------------------------------- 
	// [GetCodeInfo]�����ڵ��,�ڵ尪, �ڵ屸�� ��ȸ
	// I-PARAM : szCodeKey ����ĿŰ��(�빮��)
	//                  ��) "JMASTER", "KMASTER"
	//			 nFlag 0:�ڵ尪���� �ڵ�� ��ȸ
	//					1 : �ڵ������ �ڵ尪 ��ȸ
	//					2 : �ڵ屸�� ����
							// ���ϵǴ� �ڵ屸�а�
							//	"1"		= 	���� ���� 
							//	"2"		= 	�ڽ��� ����
							//	"3"		= 	�������� 
							//	"4"		= 	����, �������� 				
							//	"5"		= 	�ɼ� 						
							//	"8"		= 	ä�� 						
							//	"9"		= 	�������� 					
							//	"A"		= 	ETF�ڵ� 					
							//	"B"		= 	KOSPI 200 ����, KOSPI 100 ����, �峻����, KRX, KRX����
							//	"C"		= 	�ڽ��� ����, �ڽ��� 50 		
							//	"G"		= 	�����ֽĿɼ�  				
							//	"L"		= 	����������� 				
							//	"M"		= 	��Ÿ���� 					
							//	"N"		=	�ֽĿ���Ʈ ELW 	
	//			 szKey
	// O-PARAM : �ش絥����
	--------------------------------------------------- */
	STDMETHOD_(CString, GetCodeInfo)(LPCTSTR szCodeKey, short nFlag, LPCTSTR szKey) PURE;
	
	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(void, SetPosition)(LPCTSTR szKey, short nX, short nY) PURE;

	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(CString, GetMaxHangSaGa)() PURE;

	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, ShowCodeTableWithPosition)(LPCTSTR szCodeKey, const VARIANT& hWnd, short nX, short nY) PURE;

	/* ---------------------------------------------------
	//	��� :	��� ����Ʈ�� �ִ� �����͸� �����Ѵ�.
	//			������������ ������ ������ ������Ʈ �ɶ�
	//			������ ����Ʈ�� ���־߸� ���� �о� ���δ�.
	--------------------------------------------------- */
	STDMETHOD_(BOOL, RemoveAllListData)() PURE;
	
	/**********************************************************************************
	-. �� �� : App �� �ִ� m_HistoryList �� �����͸� �߰��Ѵ�.
	-. �� �� 
	short nMaxCount  : History â�� ������ List �� ����
	long nType       : 28byte(CodeInput �� CodeInput �� m_szCodeChoice �� long ��)
	LPCTSTR Code     : Code (�� : 00660 )
	LPCTSTR CodeName : Code Name ( �� : ���̴н� )
	-. ���ϰ� : ������ history �� ������ FALSE, �ƴϸ� TRUE �� �����Ѵ�.
	***********************************************************************************/
	STDMETHOD_(BOOL, InsertHistory)(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName) PURE;
	
	/**********************************************************************************
	-. �� �� : nType �� �´� History �����͸� �����Ѵ�.
	-. �� �� 
	   short nMaxCount  : History â�� ������ List �� ����
	   long nType       : 28byte(CodeInput �� CodeInput �� m_szCodeChoice �� long ��)
	-. ���ϰ�  : Ÿ�Կ� �´� �ڵ�/�ڵ���� �����͸� �����Ѵ�.
		���: "Code0=CodeName0;Code1=CodeName1;" �� �����Ѵ�.
	***********************************************************************************/
	STDMETHOD_(CString, GetHistoryList)(short nMaxCount, long nType) PURE;

	/* --------------------------------------------------- 
	-. ����
	-. I-PARAM :
	-. O-PARAM :
	--------------------------------------------------- */
	STDMETHOD_(long, GetGlobalDataTypeList)() PURE;

	/**********************************************************************************
	// ������Ű�� ���� ���񸮽�Ʈ�� �����Ѵ�.
	// I-PARAM : ������������ Ű���ε�, ���ϸ��� ��(.)���� Ȯ���ڸ� �� �빮��.
	//           MasterDefine.h ������ �����ϱ� �ٶ�.
	// O-PARAM : ���������Ͽ� ���� ����ȯ�� ����� ��.
	// "JMASTER"	---->	CList<CJMaster, CJMaster>* list = 
	***********************************************************************************/
	STDMETHOD_(long, GetCodePointer)(LPCTSTR szCodeKey) PURE;	
	
// }} Code.ocx
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// ��������� Code.ocx�� ���õ� �κ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// �ֽ����񸶽��Ϳ��� ������ �ش��ϴ� �����ϱ�
	// ���Ϲ��� ����Ʈ pReturnList�� CList<CJMaster, CJMaster>*
//	STDMETHOD_(int, GetJMaster_Upcode)(LPCTSTR szUpcode, long pReturnList) PURE;	

	// �ڽ������񸶽��Ϳ��� ������ �ش��ϴ� �����ϱ�
	// ���Ϲ��� ����Ʈ pReturnList�� CList<CJMaster, CJMaster>*
//	STDMETHOD_(int, GetKMaster_Upcode)(LPCTSTR szUpcode, long pReturnList) PURE;	


//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// ������� KOSCOM�� ���õ� �κ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	STDMETHOD_(BOOL, GetOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, 
		CStringArray *&pastrJongEngName, CStringArray *&pastrEventPrice, 
		CStringArray *&pastrFinalYM, CStringArray *&pastrValueGubun )	PURE;

	// Koscom ���������͵����� ���ϱ�
	STDMETHOD_(BOOL, GetFutureJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;

	// Koscom �ڽ��Ǿ��������͵����� ���ϱ�
	STDMETHOD_(BOOL, GetKSPUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;

	// Koscom �ڽ��ھ��������͸����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetKSQUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;

	// �ؿ����� �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetFRMst)(CStringArray *&pastrCode, CStringArray *&pastrName) PURE;

	// �ؿܱ��� �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetFRNationMst)(CStringArray *&pastrCode, CStringArray *&pastrName) PURE;

	// ELW �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetELWMst)(CStringArray *&pastrCode, CStringArray *&pastrName) PURE;

	// �̴� ���� �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetMiniFutureJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
	
	// �̴� �ɼ� �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetMiniOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName ) PURE;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	//{{ For JMaster/KMaster
	// ���� ���Ǵ� JMASTER�� KMASTER�� ���ؼ� �߰��� ���
	// ����
	// 1. LoadMASTER_J_K(..)
	// 2. J_K_GetMaster(..)
	//    J_K_SortByName(..)
	//    J_K_SortByCode(..)
	// 3. UnLoadMASTER_J_K(..)
	// �ε�� ������ �ϴ��� �ѹ��� �ε�Ǹ�,
	// ��ε�� �θ��� �ʴ��� �ڵ������� ��������.
	/**********************************************************************************
	-. �� �� : [LoadMASTER_J_K]JMASTER�� KMASTER�� �о���δ�.
				�̶� �о���̴� ���´� CMaster_J_K �����̸�,
				������ �����ڵ� ������ ������ �����Ѵ�.
	-. �� �� :
	-. ���ϰ�  : ����Count
	***********************************************************************************/
//	STDMETHOD_(int,  LoadMASTER_J_K)() PURE;

	/**********************************************************************************
	-. �� �� : [UnLoadMASTER_J_K]
	-. �� �� :
	-. ���ϰ�  : 
	***********************************************************************************/
//	STDMETHOD_(void, UnLoadMASTER_J_K)() PURE;

	/**********************************************************************************
	-. �� �� : [J_K_GetMaster]
	-. �� �� :
	-. ���ϰ�  : 
	***********************************************************************************/
	//STDMETHOD_(CMaster_J_K*, J_K_GetMaster)() PURE;

	/**********************************************************************************
	-. �� �� : [J_K_JMasterCount]
	-. �� �� :
	-. ���ϰ�  : 
	***********************************************************************************/
//	STDMETHOD_(int, J_K_JMasterCount)() PURE;


	/**********************************************************************************
	-. �� �� : [J_K_KMasterCount]
	-. �� �� :
	-. ���ϰ�  : 
	***********************************************************************************/
//	STDMETHOD_(int, J_K_KMasterCount)() PURE;


	/**********************************************************************************
	-. �� �� : [J_K_SortByName] �̹� �ε�� ���¿��� �̸����� ����
	-. �� �� :
	-. ���ϰ�  : 
	***********************************************************************************/
//	STDMETHOD_(CMaster_J_K*, J_K_SortByName)() PURE;

	/**********************************************************************************
	-. �� �� : [J_K_SortByCode] �̹� �ε�� ���¿��� �ڵ�� ����
	-. �� �� :
	-. ���ϰ�  : 
	***********************************************************************************/
//	STDMETHOD_(CMaster_J_K*, J_K_SortByCode)() PURE;

	//}} For JMaster/KMaster

	// ���������� ��Ʈ�� ȿ�������� �� �� ������, ����Ǿ��ִ� ������ �����ϱ� ���ؼ� ���� �߰��Ѵ�.
	// szMaster�� Ư�� ������Ű �ϳ���
	// ��) "JMASTER"
//	STDMETHOD_(void, CodeSort)(LPCSTR szMaster, void* pList ,int nCount) PURE;


	// --- ���� �ȵ� ----
// 	STDMETHOD_(BOOL, IsKOSPI) (LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsKosdaq)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsStock) (LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsFuture)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsSpread)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsStockOption)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsOption)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsCall)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsPut)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsKP200)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, IsKQ50)(LPCSTR szCode) PURE;
// 	STDMETHOD_(BOOL, CheckKP200Sect)(LPCSTR szCode) PURE;
	// STDMETHOD_(int, GetItemGubun(LPSTR lpszInfo));
	// int static		GetFutOptMarketGubun(CString code);
	//static CString	GetExcercisePrice(int nGubun, CString szData);
	//static CString	GetExpiryMonth(int nGubun, CString szData);
	// static BOOL		CheckItem(int nCheckFlag,CString szItemSect);
	// �׷�� �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetGroupMst)(CStringArray *&pastrCode, CStringArray *&pastrName, int nOption=0) PURE;
	STDMETHOD_(BOOL, GetKRXUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;
	STDMETHOD_(CString, GetStockDetailInfo)(CString strCode, int nType) PURE;
	STDMETHOD_(CString, GetLastCode)(CString strMarketCode) PURE;

	// Koscom �ڽ��Ǿ��������͵����� ���ϱ�
	STDMETHOD_(BOOL, GetK200UpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
	STDMETHOD_(BOOL, GetK100UpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
	STDMETHOD_(BOOL, GetKSTRUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
	STDMETHOD_(BOOL, GetFREEUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;

	// �ؿ�
	STDMETHOD_(BOOL, GetForeignMst)(int nKind, CString strCode, CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
	// FX
	STDMETHOD_(BOOL, GetFXMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName) PURE;
// 	STDMETHOD_(BOOL, GetUSUPMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetDow30Master)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetNasdaq100Master)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetSP500Master)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetCMEFutMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetETCFutMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetMoneyRateMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetLiborRateMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetADRMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetGDRMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetGovMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetSemiMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;
// 	STDMETHOD_(BOOL, GetChangeRateMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) PURE;


	// �̴� �ɼ� �����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetWeeklyOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName ) PURE;

};


//DECLARE_INTERFACE_(IMasterDataManager0712, IMasterDataManager)
DECLARE_INTERFACE_(IMasterDataManager080902, IMasterDataManager)
{
	enum
	{
		IMaster_Future=10,
		IMaster_Option,
	};
	// ����,�ɼ� �����ڵ� ����������� ��� Ȯ���� ���ؼ� ...
	// ����� ������ FALSE�̰� rszCode�� ���� �ֱٿ��� ����.
	// ������� ���������� TRUE�����ϰ� rszCode�� szCode�� �״�� �Ѱ���.
	STDMETHOD_(BOOL, GetRecentCode)(CString szCode, CString& rszCode, int nCodeType) PURE;

//DECLARE_INTERFACE_(IMasterDataManager080902, IMasterDataManager0712)
//	STDMETHOD_(BOOL, GetMasterData)(CString szType, CStringArray &arrCode, CStringArray &arrName) PURE;
	STDMETHOD_(BOOL, GetMasterDataInUpDetails)(CString szType, CStringArray &arrCode, CStringArray &arrBuf, CString strUp) PURE;
	STDMETHOD_(BYTE, GetMarketTypeFromCode)(LPCSTR strCode) PURE;
//	STDMETHOD_(CString, GetJongmokNameFromCode)(CString strCode) PURE;
	STDMETHOD_(BOOL, GetRecentCodeNameArray)(int nMarketType, CStringArray &arrCode, CStringArray &arrName) PURE;
	STDMETHOD_(BOOL, GetAllStock)(CStringArray &arrCode, CStringArray &arrName) PURE ;
	STDMETHOD_(BOOL, GetELWIndexMaster)(CStringArray &arrCode, CStringArray &arrName) PURE ;
	STDMETHOD_(BOOL, GetELWIssueMaster)(CStringArray &arrCode, CStringArray &arrName) PURE ;
	STDMETHOD_(BOOL, GetJFMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
	STDMETHOD_(BOOL, GetJPMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
	STDMETHOD_(BOOL, GetELWIndexCode)(CString strELWCode, CString& strIndexCode, CString& strIndexName) PURE;
	STDMETHOD_(CString, GetCodeName)(LPCTSTR lpszItemCode) PURE;
	STDMETHOD_(CString, GetItemExpCode)(LPCTSTR lpszCode) PURE;

//DECLARE_INTERFACE_(IMasterDataManager080299, IMasterDataManager080331)
	STDMETHOD_(BOOL, GetThemeGroup)(CStringArray& arCode, CStringArray& arName) PURE;
	STDMETHOD_(BOOL, GetThemeItemList)(LPCTSTR szThemeCode, CStringArray &arrCode, CStringArray &arrName) PURE;

//DECLARE_INTERFACE_(IMasterDataManager080902, IMasterDataManager080299)
// 	STDMETHOD_(BOOL, GetAllStock)(CStringArray &arrCode, CStringArray &arrName) PURE;
 	STDMETHOD_(BOOL, GetStockCodeWithType)(int nType, CStringArray &arrCode, CStringArray &arrName) PURE ;			// 2008.11.11 -hidden	
// 	STDMETHOD_(BOOL, GetJFMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
// 	STDMETHOD_(BOOL, GetJPMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
// 	STDMETHOD_(BOOL, GetFreeBoardMst)(CStringArray &arrCode, CStringArray &arrName) PURE;
// 	STDMETHOD_(BOOL, GetELWIndexMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
// 	STDMETHOD_(BOOL, GetELWIssueMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;

	//@Solomon������ ���
	STDMETHOD_(BOOL, GetSiteUpKode)(LPCSTR szCode, CString& rCode) PURE;

	STDMETHOD_(BYTE, GetCodeTypeFromCode)(LPCSTR strCode)PURE;

	// �ڳؽ����������͸����͵����� ���ϱ�
	STDMETHOD_(BOOL, GetKNXUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;
	STDMETHOD_(BOOL, GetETNMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;
	STDMETHOD_(BOOL, GetVFutureMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;
	STDMETHOD_(BOOL, GetSFutureMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) PURE;
//	STDMETHOD_(BOOL, GetFOFLMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
//	STDMETHOD_(BOOL, GetFOFLSubMaster)(CStringArray &arrCode, CStringArray &arrName) PURE;
};


#define	USERINFOMNG_USERID			0
#define	USERINFOMNG_USERENCID		1
#define	USERINFOMNG_ROOTDIR			10
#define	USERINFOMNG_DATADIR			12
#define	USERINFOMNG_IMAGEDIR		13
#define	USERINFOMNG_USERDIR			14
#define	USERINFOMNG_REALUSERDIR		17


#endif //AFX_INTERFACE_OF_MASTERDATAMANAGER_BY_JUNOK_LEE__INCLUDED_
