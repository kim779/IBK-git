#include "stdafx.h"
#include "PcTrMng.h"
//#include "../CommonTR/SSSTOrderHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CCompanyManager
CCompanyManager::CCompanyManager()
{
//	m_SysWnd.m_nSysEde = -1;
	m_nSysEdu = -1;
}

CCompanyManager::~CCompanyManager()
{
}

int CCompanyManager::GetID()
{
	return gCURRENT_COMPANY_ID;
}

LPCSTR CCompanyManager::GetName(int nID)
{
	char *aIDName[]={"DAISHIN",
					"DAEWOO"	,	//
					"HYUNDAI"	,	//����
					"WOORI"		,	//�츮
					"HANWHA"	,	//��ȭ
					"DONGBU"	,	//����
					"SK	"		,	//SK	
					"KYOBO"		,	//����
					"DONGWON"	,	//����
					"KIWOOM"	,	//Ű��
					"SEOUL"		,	//����
					"SAMSUNG"	,	//�Ｚ
					"BOOKOOK"	,	//�α�
					"HANTU"}	;	//����

	return aIDName[nID];
}

// ---------------------------------------------------------------------------------------
//
//		�Ʒ� ��ƾ�� �Ｚ���õ� ��ƾ��.	[START]
//		�ٸ� ����Ʈ������ �ʿ信���� ������ Delete
//
// ---------------------------------------------------------------------------------------
int CCompanyManager::GetAcctCode(CStringList& listAcct, CStringList& listAcctName)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG) return GetSamsungAcctCode(listAcct, listAcctName);
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE) return GetEugeneAcctCode(listAcct, listAcctName);
	return 0;
}


//int CCompanyManager::GetAcctCodeEx(CStringList& listAcct, CStringList& listAcctName, CStringList& listAcctType, CStringList& listAcctTypeName)
int CCompanyManager::GetAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		return GetSamsungAcctCodeEx(ListAcctInfo, nReqType);
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		return GetEugeneAcctCodeEx(ListAcctInfo, nReqType);

	return 0;
}

LPCSTR CCompanyManager::GetAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType)
{
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
		return GetSamsungAcctInfo(szAcct, nAcctSize, nInfoType);
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
		return GetEugeneAcctInfo(szAcct, nAcctSize, nInfoType);
	return "";
}

// ��  ��  ��  : KSLink_GetAccInfo
// ��      ��  : ���������� ���Ѵ�.
// ��      ��  : CString strAccount : ���¹�ȣ
//		int nType : 
//		#define		ACC_SERIAL			0		// ������ǰ�Ϸù�ȣ(3)
//		#define		ACC_PWD				1		// ���º�й�ȣ  Passwd(8)
//		#define		ACC_CODE_NAME		2		// ���¸�(20)
//		#define		ACC_PRODUCTCODE		3		// ������ǰ�ڵ�(3)
//		#define		ACC_PRODUCNAME		4		// ������ǰ������(30)
//		#define		ACC_TYPECODE		5		// ���°��������ڵ�(3)
//		#define		ACC_LAPTYPECODE		6		// �������ڵ�(1)(�ֹ��� �������� ����, ���� ��ȸ�� ����)
//		#define		ACC_OLDACCTYPECODE	7		// �����±��л��ڵ�(2)
//		#define     ACC_STIPYN          8		// ���վ��Ȯ�ο���(1)
//		#define		ACC_ISALL			9		// ���տ���(1)
//		#define		ACC_ISCLOSE			10		// ��⿩��(1) 
//		#define		ACC_HTS				11		// HTS����ޱ����ڵ�(1)
//		#define		ACC_ISPWD			12		// ���
int CCompanyManager::GetSamsungAcctCode(CStringList& listAcct, CStringList& listAcctName)
{
//@��������, ������ ����ü�迡 �°� �����ؾ���.
/*	int nAccCount = theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		CString		strAccCode;
		strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );

		if(strAccCode.GetLength() == 11)
		{
			strInfo[ACC_CODE_NAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_CODE_NAME);
			strInfo[ACC_PRODUCNAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_PRODUCNAME);


			strAccName = strInfo[ACC_CODE_NAME];

			strAccCode = strAccCode.Left(8) + "[" + strInfo[4] + "]" + strAccName;
			listAcctName.AddTail(strAccName);
			listAcct.AddTail(strAccCode);
		}
	}
	return listAcctName.GetCount();
*/ return 0;
}


int CCompanyManager::GetSamsungAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType)
{
//@��������, ������ ����ü�迡 �°� �����ؾ���.
/*	if(nReqType==-1)
		return theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();

	int nAccCount = theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		STACC_SSDATA* pNewItem = new STACC_SSDATA;
		CString		strAccCode;
		strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
		
		pNewItem->szAcct = strAccCode;
		for(int j=0; j<=ACC_ISPWD; j++)
			pNewItem->InData[j] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , j);

		ListAcctInfo.AddTail(pNewItem);
	}

	return nAccCount;
*/
	return 0;
}

LPCSTR CCompanyManager::GetSamsungAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType)
{
//@��������, ������ ����ü�迡 �°� �����ؾ���.
/*	if(nAcctSize==11)
	{
		theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( szAcct , nInfoType);
		return (LPCSTR)theApp.m_szTemp;
	}
	else	// ��ü���¿��� Ȯ��
	{
		int nAccCount = theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
		CString		strAccCode;
		for ( int i = 0; nAccCount > i ; i++)
		{
			strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
			if(strncmp(strAccCode, szAcct, nAcctSize)==0)
			{
				theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , nInfoType);
				return (LPCSTR)theApp.m_szTemp;
			}
		}
	}
*/	return "";
}

CString CCompanyManager::GetPwd()
{
	return "";
//	return theApp.m_KSLibMng.Lib_KSLink_GetLoginInfo( "USER_PASSWORD" );
}

/*
long CCompanyManager::OnGetData(WPARAM wParam, LPARAM lParam)
{
	CRpDataParser rpParser;
	rpParser.OnGetData ( wParam, lParam );
	if(  rpParser.IsError() )
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//����ó��
		int nIndex   = rpParser.GetPacketID();
		// char *pErrMsg = rpParser.GetErrMsg();
		CString strError;
		strError.Format("[%s] ", rpParser.GetErrCode());
 		strError += rpParser.GetErrMsg();

		//CString strMsg = pErrMsg;
		//TRACE("\n(%s) %s\n", "CCompanyManager::OnGetData", strMsg);
		AfxMessageBox(strError);

		return 0L;
	}

	////////////////////////////////////////////////////////////////////////////
	//tr
	//�����ܰ���ȸ
	CString strTrCode = rpParser.GetTranCode();
	strTrCode.TrimRight();

	m_szLastMsgCode = rpParser.GetReciveCode();
	m_szLastMsg = CommGetMsgString(m_szLastMsgCode);
	// m_Form.SetFrameMsgText(strMsg);

	return 1L;
}

BOOL CCompanyManager::GetLastMessage(CString& szMsgCode, CString& szMsg)
{
	szMsg = m_szLastMsg;
	szMsgCode = m_szLastMsgCode;
	return szMsgCode.GetLength();
}
*/

//*****************************************************************************
// ��  ��  ��  : int CCompanyManager::GetSysEdu(LPCSTR szMainKey, long pThisPoint, long dwSubKey, SysEduCallbackProc pCallFunc)
// ��      ��  : �ý���Ʈ���̵� �������θ� üũ�Ѵ�.
// ��      ��  : 
// ȣ��  �Լ�  : ���ϰ� : 0(�̱���), 1(�����Ϸ�), 0���ϰ�(��ȸ������)
// ��  ��  ��  : ���ؿ�     (��)Ʃ����ó��
// Date        : 2007-01-22 ���� 7:52:35
//*****************************************************************************
int CCompanyManager::GetSysEdu(LPCSTR szMainKey, long pThisPoint, long dwSubKey, SysEduCallbackProc pCallFunc)
{
	// �ý���Ʈ���̵� ��������. 0�̻��̸� ���õ�. 0��������. 1������.
	//if(m_SysWnd.m_nSysEde>=0)
	if(gCURRENT_COMPANY_ID==nPCTRComID_SAMSUNG)
	{
		gGetSamsungSysEdu(m_nSysEdu);
	}
	else if(gCURRENT_COMPANY_ID==nPCTRComID_EUGENE)
	{
//		gGetEugeneSysEdu(m_nSysEdu);
		m_nSysEdu = 1;		// �켱 �����̼�ó��
//		m_nSysEdu = atoi(theApp.m_KSLibMng.Lib_KSLink_GetLoginInfo( "SYSTRAD_AGR" )); // �ý��� Ʈ���̵���뵿�ǿ���
		CString szTmp; szTmp.Format("%d", m_nSysEdu);
		::SetEnvironmentVariable(SZEUGENE_SYSEDU, szTmp);
	}

	if(m_nSysEdu>=0)
	{
		//pCallFunc(pThisPoint, szMainKey, dwSubKey, m_SysWnd.m_nSysEde);
		//return m_SysWnd.m_nSysEde;
		return m_nSysEdu;
	}

/*
	if(m_SysWnd.m_hWnd==NULL)
	{
		if(!CreateChkSysWnd())
		{
			// ���� Callback����
			//pCallFunc(pThisPoint, szMainKey, dwSubKey, m_SysWnd.m_nSysEde);
			//return m_SysWnd.m_nSysEde;
			return m_nSysEdu;
		}
	}
	m_SysWnd.SendV840Data();

	if(pCallFunc)
	{
		STSysCallProcItem* pItem = new STSysCallProcItem;
		pItem->szMainKey = szMainKey;
		pItem->pThisPoint = pThisPoint;
		pItem->dwSubKey = dwSubKey;
		pItem->dwNotifyData = (long)pCallFunc;
		
		m_SysWnd.m_ListCallback.AddTail(pItem);
	}
*/

	//return m_SysWnd.m_nSysEde;
	return m_nSysEdu;
}

void CCompanyManager::RemoveSysEdu(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc)
{
//	m_SysWnd.RemoveSysEdu(szKey, pThisPoint, dwKey, pCallFunc);
}

BOOL CCompanyManager::CreateChkSysWnd()
{
	return FALSE;
//	return m_SysWnd.OpenWindow();
}

// ---------------------------------------------------------------------------------------
//
//		��������� �Ｚ���õ� ��ƾ��.	[END]
//
// ---------------------------------------------------------------------------------------


// ��  ��  ��  : KSLink_GetAccInfo
// ��      ��  : ���������� ���Ѵ�.
// ��      ��  : CString strAccount : ���¹�ȣ
//		int nType : 
//		#define		ACC_SERIAL			0		// ������ǰ�Ϸù�ȣ(3)
//		#define		ACC_PWD				1		// ���º�й�ȣ  Passwd(8)
//		#define		ACC_CODE_NAME		2		// ���¸�(20)
//		#define		ACC_PRODUCTCODE		3		// ������ǰ�ڵ�(3)
//		#define		ACC_PRODUCNAME		4		// ������ǰ������(30)
//		#define		ACC_TYPECODE		5		// ���°��������ڵ�(3)
//		#define		ACC_LAPTYPECODE		6		// �������ڵ�(1)(�ֹ��� �������� ����, ���� ��ȸ�� ����)
//		#define		ACC_OLDACCTYPECODE	7		// �����±��л��ڵ�(2)
//		#define     ACC_STIPYN          8		// ���վ��Ȯ�ο���(1)
//		#define		ACC_ISALL			9		// ���տ���(1)
//		#define		ACC_ISCLOSE			10		// ��⿩��(1) 
//		#define		ACC_HTS				11		// HTS����ޱ����ڵ�(1)
//		#define		ACC_ISPWD			12		// ���
int CCompanyManager::GetEugeneAcctCode(CStringList& listAcct, CStringList& listAcctName)
{
	int nAccCount = 0;//theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		CString		strAccCode;
		strAccCode	= "";//theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );

		if(strAccCode.GetLength() == 11)
		{
/*
			strInfo[ACC_CODE_NAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_CODE_NAME);
			strInfo[ACC_PRODUCNAME] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , ACC_PRODUCNAME);
*/


			strAccName = strInfo[ACC_CODE_NAME];

			strAccCode = strAccCode.Left(8) + "[" + strInfo[4] + "]" + strAccName;
			listAcctName.AddTail(strAccName);
			listAcct.AddTail(strAccCode);
		}
	}
	return listAcctName.GetCount();
}


int CCompanyManager::GetEugeneAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType)
{
//	if(nReqType==-1)
//		return theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();

	int nAccCount = 0;//theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
	CString		strInfo[ACC_ISPWD+1];
	CString		strAccName;
	for ( int i = 0; nAccCount > i ; i++)
	{
		STACC_SSDATA* pNewItem = new STACC_SSDATA;
		CString		strAccCode;
//		strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
		
		pNewItem->szAcct = strAccCode;
//		for(int j=0; j<=ACC_ISPWD; j++)
//			pNewItem->InData[j] = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , j);

		ListAcctInfo.AddTail(pNewItem);
	}

	return nAccCount;
}

LPCSTR CCompanyManager::GetEugeneAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType)
{
	if(nAcctSize==11)
	{
//		theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( szAcct , nInfoType);
		return (LPCSTR)theApp.m_szTemp;
	}
	else	// ��ü���¿��� Ȯ��
	{
		int nAccCount = 0;//theApp.m_KSLibMng.Lib_KSLink_GetAccountCount();
		CString		strAccCode;
		for ( int i = 0; nAccCount > i ; i++)
		{
//			strAccCode	= theApp.m_KSLibMng.Lib_KSLink_GetAccount( i );
			if(strncmp(strAccCode, szAcct, nAcctSize)==0)
			{
//				theApp.m_szTemp = theApp.m_KSLibMng.Lib_KSLink_GetAccInfo( strAccCode , nInfoType);
				return (LPCSTR)theApp.m_szTemp;
			}
		}
	}
	return "";
}