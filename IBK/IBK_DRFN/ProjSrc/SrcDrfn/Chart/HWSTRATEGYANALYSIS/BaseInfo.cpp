// BaseInfo.cpp: implementation of the CBaseInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "interestdll.h"
#include "BaseInfo.h"

// start
#include "../../../DataMgr/DBMgr.h"
#include "../../../include/EnvDir.h"
#include "../../../include/Defines.h"				// HTS ������ Eagle ������ ������ �ʿ信 ���� �߰���
#include "../../../MapDB/MapDBMgr.h"
#include "../../../CtlStd/SelComboObj.h"
#include "../../../CtlStd/CodeComboObj.h"
#include "../../../CtlStd/FundAccountComboObj.h"
#include "../../../CtlStd/SmartAccountComboObj.h"	// Eagle ���� ���� ��ǲ 
// end

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseInfo::CBaseInfo()
{
	m_hParent = NULL;
	m_pMapDB = NULL;

	// -----------------------------------------
	m_pSelCode = NULL;
	m_pAcctInput = NULL;
	// -----------------------------------------
}

CBaseInfo::~CBaseInfo()
{
	CloseInfo();
}

void CBaseInfo::CloseInfo()
{
	
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}

	if(m_pAcctInput)
	{
		delete m_pAcctInput;
		m_pAcctInput = NULL;
	}
	

	// -----------------------------------------

	if(m_pMapDB)
	{
		delete m_pMapDB;
		m_pMapDB = NULL;
	}
		
}

void CBaseInfo::InitInfo(void* pDBMgr)
{
	m_pDBMgr = (CDBMgr*)pDBMgr;
	

	GET_MAIN_DIR(m_sBasePath);
	CLogUserInfo* pUserInfo = m_pDBMgr->GetLogUserData();
	m_sUserID = pUserInfo->strUserID;
}

CString CBaseInfo::GetUserPath()
{
	return (m_sBasePath + "\\User\\" + m_sUserID);
}

CString CBaseInfo::GetMainPath()
{
	return m_sBasePath;
}

HWND CBaseInfo::GetSocketHWND()
{
	return m_pDBMgr->GetCommHWnd();	
}

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/24
// Comments		: Code �����Լ��� 
//START------------------------------------------------------------------------
BOOL CBaseInfo::GetCodeInfo(DWORD nCode, CStringArray& arCode, CStringArray& arName)
{
	return m_pDBMgr->GetMasterData(nCode, arCode, arName);
}

DWORD CBaseInfo::GetCodeType(LPCSTR szCode, int nECN)
{
	return m_pDBMgr->GetCodeType(szCode,nECN);
}

BOOL CBaseInfo::GetCodeName(DWORD wType, LPCTSTR szCode, CString &strName, BOOL bFullName , BOOL bAlert )
{
	return m_pDBMgr->GetCodeName(wType,szCode,strName,bFullName,bAlert);
}

void CBaseInfo::GetCodeGroup(CString sGroup, CStringArray& arCode, CStringArray& arName)
{
	m_pDBMgr->GetGroupJongMokData(sGroup, arCode, arName);
}

BOOL CBaseInfo::GetCodeIntType(DWORD nType)
{
	switch(nType)
	{
	case CODE_JONGMOK : 			
	case CODE_KJONGMOK : 
	case CODE_FUTURE : 
	case CODE_OPTION : 
	case CODE_JOPTION : 
	case CODE_OTCBB : 
	case CODE_QFUTURE : 
	case CODE_QOPTION : 
	case CODE_EXCHANGE : 
	case CODE_EXCH_AUP : 
	case CODE_EXCH_AJONG : 
	case CODE_EXCH_DR :
		return TRUE;
	}
	return FALSE;
}

int CBaseInfo::GetCodeTRType(DWORD nType)
{
	switch(nType)
	{
	case CODE_JONGMOK : return CODEN_KOSPI; 			
	case CODE_KJONGMOK : return CODEN_KOSDAQ;
	case CODE_FUTURE : return CODEN_FUTURE;
	case CODE_OPTION : return CODEN_OPTION;
	case CODE_JOPTION : return CODEN_SOPTION;
	case CODE_OTCBB : return CODEN_THIRD;
	case CODE_QFUTURE : return CODEN_QFUTURE;
	case CODE_QOPTION : return CODEN_QOPTION;
	case CODE_EXCHANGE : return CODEN_GINDEX;
	case CODE_EXCH_AUP : return CODEN_AINDEX;
	case CODE_EXCH_AJONG : return CODEN_AITEM;
	case CODE_EXCH_DR : return CODEN_DRITEM;
	}
	return 0;
}
//END------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/24
// Comments		: �������� ����, �׸�, �׹��� �з��� �ش��ϴ� �����ڵ���� ��û
//-----------------------------------------------------------------------------
void* CBaseInfo::GetCurTrHWS09501(int nType, int nCode)
{
	CString szMarket;
	switch(nType)
	{
	case E_TREE_2KOSPI :
	case E_TREE_3KOSDOQ :
		szMarket.Format("%03d",nCode);
		return GetCurTrHWS09501(szMarket,"");
	case E_TREE_5THEME:
		szMarket.Format("%3d",nCode);
		return GetCurTrHWS09501(szMarket,"0712");
	}
	return NULL;
}

void* CBaseInfo::GetCurTrHWS09501(LPCSTR szMarket, LPCSTR szCode)
{
	m_Msg.ResetData();
	m_Msg.AddString("HWS09501", QRYCODE_LEN);
	m_Msg.AddString("N0010000", 8);
	if( strlen(szCode) == 0)
	{
		m_Msg.AddLong(4 + 4 * 2);
		m_Msg.AddLong(1);
		m_Msg.AddString("0101", 4);	// �ڵ�
		m_Msg.AddString(szMarket, 4);		// ���屸��
	}
	else
	{
		m_Msg.AddLong(4 + 4 * 2 + strlen(szCode));

		//����Ÿ ä���
		m_Msg.AddLong(2);
		m_Msg.AddString("0101", 4);	// �ڵ�
		m_Msg.AddString(szMarket, 4);		// ���屸��
		m_Msg.AddString(szCode, strlen(szCode));
	}
	return (void*)&m_Msg;
}

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/24
// Comments		: ���� ��� �� ���� ���õ� �Լ��� 
//START-------------------------------------------------------------------------
void CBaseInfo::AddRealUpdate(CMapStringToPtr *pKeyMap, HWND hWnd)
{
	m_pDBMgr->AddRealUpdate(pKeyMap,hWnd);
}

void CBaseInfo::AddRealUpdate(LPCTSTR szKey, HWND hWnd)
{
	m_pDBMgr->AddRealUpdate(szKey,hWnd);
}

void CBaseInfo::ResetRealUpdate(CMapStringToPtr *pKeyMap, HWND hWnd)
{
	m_pDBMgr->ResetRealUpdate(pKeyMap,hWnd);	
}

void CBaseInfo::ResetRealUpdate(LPCTSTR szKey, HWND hWnd)
{
	m_pDBMgr->ResetRealUpdate(szKey,hWnd);
}

WORD CBaseInfo::GetRealType(long lParam)
{
	CRealData *pData = (CRealData *)lParam;
	return pData->wRecordID;
}
//START-------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Author		: Geun-moon Oh	Date :2005/7/11
// Return void				: 
// Comments					: Delete Control CodeInput 
//-----------------------------------------------------------------------------
void CBaseInfo::DestoryControl_CodeInput()
{
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}
}

//-----------------------------------------------------------------------------
// Author					: Geun-moon Oh	Date :2005/7/11
// Return void				: 
// Comments					: Delete Control FutOptCodeCombo
//-----------------------------------------------------------------------------
void CBaseInfo::DestoryControl_FutOptCodeCombo()
{
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}
}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return void				: 
// Param  HWND hWnd			: Parent Handle
// Param  CRect rc			: CodeInput �� ������ ��ġ ( Bottom ���� �����丮����Ʈ �������� �����ؼ� �����Ѵ�. )
// Param  WORD nID /*= 0*/	: �ڵ���ǲ�� ���ҽ� ID �� �ѱ��.
// Comments					: 
//-----------------------------------------------------------------------------
void CBaseInfo::CreateControl_CodeInput(HWND hWnd, CRect rc, WORD nID /*= 0*/, 
										long lcodetype /*=CODE_JONGMOK|CODE_KJONGMOK*/,
										long lcodeMaxLength /*=6*/)
{

	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);

	//if( (lcodetype&CODE_FUTURE == CODE_FUTURE) || (lcodetype&CODE_OPTION == CODE_OPTION) )
/*	if(lisFut == CODE_FUTURE  || lisOpt == CODE_OPTION)
	{
		m_pSelCode= new CCodeComboObj;
		m_pSelCode->SetObjMode(1);
		CWnd* pWnd = CWnd::FromHandle(m_hParent);
		m_pSelCode->SetParentFrameWnd(pWnd);
		m_pSelCode->SetMapDB(m_pMapDB);
		
		m_pSelCode->CreateControl(nID,pWnd,rc);

		CPropData propdata;

		//------------------------------------------------------------------------->>
		// Create History Button
		propdata.lData = 1;
		m_pSelCode->SetProperty(PI_USER+6 , &propdata);

		//------------------------------------------------------------------------->>
		// �ڵ���ǲ���� ����� �ʵ� ������ ����
		propdata.strData	= "�����ڵ�";
		m_pSelCode->SetProperty(PI_FIDITEM , &propdata);

		
		//------------------------------------------------------------------------->>
		// �ʿ信 ���Ͽ� SelComboObj.cpp �� ���Ƿ� �߰��� �κ� ����
		// �ڵ���ǲ���� ����� �ڵ� Ÿ�� ����
		m_pSelCode->SetProperty("DataType" , "KFut|KOpt");

		
		//------------------------------------------------------------------------->>
		// �����ڵ��� �ִ� ���� ����
		propdata.lData = 8;
		m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);
	}
	else
	{
*/
		// If object exists, delete it.  By Gm0604 2005.7.11
		if(m_pSelCode)
		{
			delete m_pSelCode;
			m_pSelCode = NULL;
		}
		// end. By Gm0604 2005.7.11
		m_pSelCode= new CSelComboObj;
		m_pSelCode->SetObjMode(1);
		CWnd* pWnd = CWnd::FromHandle(m_hParent);
		m_pSelCode->SetParentFrameWnd(pWnd);
		m_pSelCode->SetMapDB(m_pMapDB);
		
		m_pSelCode->CreateControl(nID,pWnd,rc);

		CPropData propdata;

		//------------------------------------------------------------------------->>
		// Create History Button
		propdata.lData = 1;
		m_pSelCode->SetProperty(PI_USER+6 , &propdata);			// History Button

		long lisKospi = lcodetype&CODE_JONGMOK;
		long lisKosdaq = lcodetype&CODE_KJONGMOK;
		if(lisKospi == CODE_JONGMOK  || lisKosdaq == CODE_KJONGMOK)
		//if((lcodetype&CODE_JONGMOK == CODE_JONGMOK) || (lcodetype&CODE_KJONGMOK == CODE_KJONGMOK))
			m_pSelCode->SetProperty(PI_USER+13 , &propdata);		// "S" Button	
		

		m_pSelCode->SetProperty(PI_USER+14 , &propdata);		// "?" Button
		//m_pSelCode->SetProperty(PI_USER+15 , &propdata);		// "M" Button	

		//------------------------------------------------------------------------->>
		// �ڵ���ǲ���� ����� �ʵ� ������ ����
		propdata.strData	= "�����ڵ�";
		m_pSelCode->SetProperty(PI_FIDITEM , &propdata);

		
		//------------------------------------------------------------------------->>
		// �ʿ信 ���Ͽ� SelComboObj.cpp �� ���Ƿ� �߰��� �κ� ����
		// �ڵ���ǲ���� ����� �ڵ� Ÿ�� ����
		propdata.lData = lcodetype;
		m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);


		//------------------------------------------------------------------------->>
		// �����ڵ��� �ִ� ���� ����
		propdata.lData = lcodeMaxLength;
		//m_pSelCode->SetProperty(PI_USER+2 , &propdata);	-> �̷��� �����ϸ� �ؽ�Ʈ �Է½� ������ ��
		m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);


		//------------------------------------------------------------------------->>
		// DR Mode Setting -> �ڵ� ����� �޽����� �˷��ֵ��� �ϱ� ���� 
		// CChkSelCombo.OnChange -> 
		//if (m_bDRMode)
		//{
		//	CString	code;		// ssj 20020424
		//	code.Format("%s", m_curJongCode);			// ssj
		//	GetParent()->SendMessage(WMU_SET_KEYCODE , GetDlgCtrlID(), (LPARAM)(LPSTR)(LPCTSTR)code);
		//}
		propdata.lData	= 1;
		m_pSelCode->SetProperty("SetUserDefMode", &propdata);
//	}
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return void				: 
// Param  CString strCode	: ���� �ڵ�  ex)"000660"
// Comments					: ��ȭ �ڵ� ��ǲ�� ���� �ڵ带 �����Ѵ�.
//-----------------------------------------------------------------------------
void CBaseInfo::SetTextToCodeInput(CString strCode)
{
	if(m_pSelCode)
	{
		m_pSelCode->SetText(strCode);
	}
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return CString	: ������ ���� �ڵ带 �����Ѵ�.
// Comments			: ��ȭ �ڵ� ��ǲ�� ���� ���� �Ǿ� �ִ� ���� �����´�.
//-----------------------------------------------------------------------------
CString CBaseInfo::GetTextToCodeInput()
{
	if(m_pSelCode)
	{
		return m_pSelCode->GetText();
	}
	else
		return "";
}

//void CBaseInfo::SetFocusToCodeInput()
//{
//	if(m_pSelCode)
//	{
//		CPropData propdata;
//		m_pSelCode->SetProperty("SetFocusForDR", &propdata);
//	}
//}



void CBaseInfo::CreateControl_AcctInput(HWND hWnd, CRect rc, WORD nID /*= 0*/,
										long lAcctTypeCnt /*=2*/,
										CString strAcctType /*="0102"*/)
{
	m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);
	

#ifdef __INFO			// - Eagle ����
	m_pAcctInput= new CSmartAccountComboObj;
#else					// - HTS ���� 
	m_pAcctInput= new CAccountComboObj;
#endif



	m_pAcctInput->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pAcctInput->SetParentFrameWnd(pWnd);
	m_pAcctInput->SetMapDB(m_pMapDB);
	

	//////////////////////////////////////////////////////////////////////////
	CPropData propdata;

	//------------------------------------------------------------------------>>
	// SetUserDefUseAccTypeList  -> ����� ���� Ÿ���� ���� -> CreateControl �ϱ� ���� ȣ�� 
	// ���� ���� 
	// 01 ��Ź
	// 02 ����
	// 03 ����
	// 04 ����
	// 05 CD
	// 06 CP
	// 07 RP
	// 08 �����ֽ�
	// 09 ���ɼ���
	// 10 Wrap
	propdata.lData = lAcctTypeCnt;				// -> ������� ����
	propdata.strData = strAcctType;				// -> 01 : ��Ź , 02 : ���� ���� ����
	m_pAcctInput->SetProperty("SetUserDefUseAccTypeList" , &propdata);
	
	//------------------------------------------------------------------------->>
	// ���¹�ȣ ���� ǥ�� ���� 
	propdata.lData = 1;
	m_pAcctInput->SetProperty("SetUserDefShowDash", &propdata);

	//------------------------------------------------------------------------->>
	// Ÿ�θ��� ���� ���� ���� 
	m_pAcctInput->SetProperty("SetUserDefOtherAcc", &propdata);
	
	//------------------------------------------------------------------------->>
	// SelectItem Index ���� 
	propdata.lData = 0;
	m_pAcctInput->SetProperty("SetUserDefPSelPos", &propdata);


	//------------------------------------------------------------------------->>
	// LogFont ���� 
	LOGFONT lfdata;
	strcpy(lfdata.lfFaceName, "����");
	lfdata.lfHeight = -12;
	lfdata.lfWidth = 0;
	lfdata.lfEscapement = 0;
	lfdata.lfOrientation = 0; 
	lfdata.lfWeight = FW_DONTCARE;
    lfdata.lfItalic = false ;    
	lfdata.lfUnderline= false;
    lfdata.lfStrikeOut= false; 
	lfdata.lfCharSet = DEFAULT_CHARSET;
    lfdata.lfOutPrecision = OUT_DEFAULT_PRECIS ;    
	lfdata.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lfdata.lfQuality = DEFAULT_QUALITY;    
	lfdata.lfPitchAndFamily = DEFAULT_PITCH;

	propdata.fontData = lfdata;
	m_pAcctInput->SetProperty("SetUserDefFontData", &propdata);


	//----------------------------------------------------------------------------->>
	// DR Mode Setting -> ���� ����� �޽����� �˷��ֵ��� �ϱ� ���� 
	// CFundAccountComboCtl.OnChange -> 
	//if (m_bDRMode)
	//{
	//	GetParent()->SendMessage(WMU_SET_KEYCODE , GetDlgCtrlID(), (LPARAM)(LPSTR)(LPCTSTR)strAcc);
	//}
	propdata.lData	= 1;
	m_pAcctInput->SetProperty("SetUserDefMode", &propdata);	
	//-----------------------------------------------------------------------------<<
	
	//////////////////////////////////////////////////////////////////////////
	m_pAcctInput->CreateControl(nID,pWnd,rc);


}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/25
// Modifier		: 
// Comments		: ���¹�ȣ ���� ����
//-----------------------------------------------------------------------------
CString	CBaseInfo::GetTextToAcctInput()
{
	if(m_pAcctInput)
	{
		CPropData propdata;
		m_pAcctInput->GetProperty(PI_TEXT, &propdata);

		return propdata.strData;
	}
	
	return "";
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/25
// Return CString		 : ���¸�
// Param  CString strAcc : ���¹�ȣ
// Comments				 : ���¸� ���� ���� 
//-----------------------------------------------------------------------------
CString CBaseInfo::GetAccountToGNameForDR(CString strAcc)
{
	if(m_pAcctInput)
	{
		CPropData propdata;
		propdata.strData = strAcc;
		
		if(m_pAcctInput->GetProperty("GetUserDefAccountToGName" , &propdata))
		{
			return propdata.strData;
		}
	}

	return "";
}

void CBaseInfo::SetEnableToAcctInput(BOOL bEnable/*=TRUE*/)
{
	return;
//	if(m_pAcctInput)
//	{
//		CPropData propdata;
//		propdata.lData = bEnable;
//
//		m_pAcctInput->SetProperty("SetEnableForDR", &propdata);
//	}
}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/6/21
// Return void : 
// Param  HWND hWnd : Parent Handle
// Param  CRect rc	: Position Rect
// Param  WORD nID	: Resource ID
// Param  CString strcodetype : Upjong Code Type
// Comments		: ���� �ڵ� �޺��� �����Ѵ�.
//-----------------------------------------------------------------------------
void CBaseInfo::CreateControl_UpjongCodeCombo(HWND hWnd, CRect rc, WORD nID/* = 0*/,CString strcodetype/*="CODE_ALLUPCODE"*/)
{
	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);

	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}

	m_pSelCode = new CSelComboObj;
	m_pSelCode->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pSelCode->SetParentFrameWnd(pWnd);
	m_pSelCode->SetMapDB(m_pMapDB);
	

	
	
	// �Ӽ� ���� 
	CPropData propdata;
	//------------------------------------------------------------------------->>
	// ���� �ڵ���ǲ���� ����� �ڵ� Ÿ�� ����
	// ��ü����			"CODE_ALLUPCODE"
	// �峻����			"CODE_UPJONG"
	// KOSPI200 ����	"CODE_K200UPJONG"
	// KOSDAQ ����		"CODE_KUPJONG"
	//m_pSelCode->SetProperty("DataType" , strcodetype);
	propdata.lData = CODE_UPJONG | CODE_KUPJONG | CODE_K200UPJONG;
	m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);
	

	//------------------------------------------------------------------------->>
	// �����ڵ��� �ִ� ���� ����
	propdata.lData = 3;
	m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);


	//------------------------------------------------------------------------->>
	// �����ڵ��޺����� ����� �ʵ� ������ ����
	propdata.strData	= "��������/�ڵ�";
	m_pSelCode->SetProperty(PI_FIDITEM , &propdata);
	//-------------------------------------------------------------------------<<

	
	m_pSelCode->CreateControl(nID,pWnd,rc);


	//------------------------------------------------------------------------->>
	//2005. 07. 11
	// DR Mode Setting -> �ڵ� ����� �޽����� �˷��ֵ��� �ϱ� ���� 
	propdata.lData	= 1;
	m_pSelCode->SetProperty("SetUserDefMode", &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	// �ʱ� ������ ���� ���õ� �����ڵ带 �����Ѵ�.
	char szBuf[20];
	int nKind = m_pSelCode->GetDataKind();
	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf))
	{
		if( szBuf[0] )
		{
			m_pSelCode->SetText(szBuf);
		}
	}
	//-------------------------------------------------------------------------<<
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/6/21
// Return CString		  : �ش� strcode �� ���� ������
// Param  CString strcode : �����ڵ�
// Param  long lcodetype  : �����ڵ� Ÿ��
// Comments				  :	�����ڵ忡 ���� �������� ��� �´�.
//				- �����ڵ� �Ӹ� �ƴ϶� �����ڵ� �����ؼ��� ��� �ü� �ִ�.
//-----------------------------------------------------------------------------
CString CBaseInfo::GetUpjongNameToCodeForDR(CString strcode , long lcodetype /*= CODE_ALLUPCODE*/)
{
	CString strName;
	
	m_pDBMgr->GetCodeName(lcodetype, strcode, strName);

	return strName;
}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/6/21
// Return void	: 
// Comments		: SelCode , AcctInput , UpjongCodeCombo ���� ���� �����͸� 
//				�����ʿ� ���ý�Ų��.(��������)		
//-----------------------------------------------------------------------------
void CBaseInfo::SetKeyCodeToMain_AllControl()
{
	int nKind;
	CString strData;

	if(m_pSelCode)
	{
		nKind = m_pSelCode->GetDataKind();
		if( nKind != CODE_NONE)
		{
			strData = m_pSelCode->GetText();
			if( !strData.IsEmpty())
			{
				AfxGetMainWnd()->SendMessage(WMU_SET_KEYCODE, (WPARAM)nKind, (LPARAM)(LPCTSTR)strData);
			}
		}
	}

	if(m_pAcctInput)
	{
		nKind = m_pAcctInput->GetDataKind();
		if( nKind != CODE_NONE)
		{
			strData = m_pAcctInput->GetText();
			if( !strData.IsEmpty())
			{
				AfxGetMainWnd()->SendMessage(WMU_SET_KEYCODE, (WPARAM)nKind, (LPARAM)(LPCTSTR)strData);
			}
		}
	}
	//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/21
// Return void : 
// Param  HWND hWnd : Parent Handle
// Param  CRect rc	: Position Rect
// Param  WORD nID	: Resource ID
// Param  CString strcodetype : Upjong Code Type
// Comments		: �ؿ����� �ڵ� �޺��� �����Ѵ�.
//-----------------------------------------------------------------------------
void CBaseInfo::CreateControl_ForeignCodeCombo(HWND hWnd, CRect rc, WORD nID/* = 0*/,CString strcodetype/*="CODE_EXCH_ALL"*/)
{
	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);


	m_pSelCode = new CSelComboObj;
	m_pSelCode->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pSelCode->SetParentFrameWnd(pWnd);
	m_pSelCode->SetMapDB(m_pMapDB);
	
	
	// �Ӽ� ���� 
	CPropData propdata;
	//------------------------------------------------------------------------->>
	// �ؿ� �ڵ���ǲ���� ����� �ڵ� Ÿ�� ����
	//CODE_EXCHANGE	 �ؿ� ��������(��:�ؿ����� �� ȯ��)	
	//CODE_EXCH_AUP	 �̱� ����/����						
	//CODE_EXCH_AJONG	 �̱� ����(DOW30, NASDAQ100, S&P500)	
	//CODE_EXCH_DR	 �ؿ� DR����							
	//m_pSelCode->SetProperty("DataType" , strcodetype);
	propdata.lData = CODE_EXCHANGE | CODE_EXCH_AUP | CODE_EXCH_AJONG | CODE_EXCH_DR;
	m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);

	//------------------------------------------------------------------------->>
	// �ؿ������ڵ��� �ִ� ���� ����
	propdata.lData = 6;
	m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);


	//------------------------------------------------------------------------->>
	// �ؿ������ڵ��޺����� ����� �ʵ� ������ ����
	propdata.strData	= "�ؿ���������/�ڵ�";
	m_pSelCode->SetProperty(PI_FIDITEM , &propdata);
	//-------------------------------------------------------------------------<<
	
	m_pSelCode->CreateControl(nID,pWnd,rc);

	//------------------------------------------------------------------------->>
	//2005. 07. 11
	// DR Mode Setting -> �ڵ� ����� �޽����� �˷��ֵ��� �ϱ� ���� 
	propdata.lData	= 1;
	m_pSelCode->SetProperty("SetUserDefMode", &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	// �ʱ� ������ ���� ���õ� �ؿ������ڵ带 �����Ѵ�.
	char szBuf[20];
	int nKind = m_pSelCode->GetDataKind();
	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf))
	{
		if( szBuf[0] )
		{
			m_pSelCode->SetText(szBuf);
		}
	}
	//-------------------------------------------------------------------------<<
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/21
// Return CString		  : �ش� strcode �� ���� �ؿ�������
// Param  CString strcode : �ؿ������ڵ�
// Param  long lcodetype  : �ؿ������ڵ� Ÿ��
// Comments				  :	�ؿ������ڵ忡 ���� �ؿ��������� ��� �´�.
//				- �ؿ������ڵ� �Ӹ� �ƴ϶� �����ڵ� �����ؼ��� ��� �ü� �ִ�.
//-----------------------------------------------------------------------------
CString CBaseInfo::GetForeignNameToCodeForDR(CString strcode , long lcodetype /*= CODE_EXCH_ALL*/)
{
	CString strName;
	
	m_pDBMgr->GetCodeName(lcodetype, strcode, strName);

	return strName;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/23
// Return CString		  : �ش� strcode �� ���� �����ɼǸ�
// Param  CString strcode : �����ɼ��ڵ�
// Param  long lcodetype  : �����ɼ��ڵ� Ÿ��
// Comments				  :	�����ɼ��ڵ忡 ���� �����ɼǸ��� ��� �´�.			
//-----------------------------------------------------------------------------

void CBaseInfo::CreateControl_FutOptCodeCombo(HWND hWnd, CRect rc, WORD nID /*= 0*/, 
										CString  strcodetype /*=CODE_FUTURE|CODE_OPTION*/,
										long lcodeMaxLength /*=8*/)
{

	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);

	// If object exists, delete it.  By Gm0604 2005.7.11
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}
	// end. By Gm0604 2005.7.11
		
	m_pSelCode= new CSelComboObj;
	m_pSelCode->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pSelCode->SetParentFrameWnd(pWnd);
	m_pSelCode->SetMapDB(m_pMapDB);
	
	m_pSelCode->CreateControl(nID,pWnd,rc);

	CPropData propdata;

	//------------------------------------------------------------------------->>
	// Create History Button
	//propdata.lData = 1;
	//m_pSelCode->SetProperty(PI_USER+6 , &propdata);		
	//-------------------------------------------------------------------------<<
	

	//------------------------------------------------------------------------->>
	// �ڵ���ǲ���� ����� �ʵ� ������ ����
	propdata.strData	= "�����ڵ�/�����ɼ�";
	m_pSelCode->SetProperty(PI_FIDITEM , &propdata);

	
	//------------------------------------------------------------------------->>
	// �ʿ信 ���Ͽ� SelComboObj.cpp �� ���Ƿ� �߰��� �κ� ����
	// �ڵ���ǲ���� ����� �ڵ� Ÿ�� ����	
	//m_pSelCode->SetProperty("DataType" , strcodetype);
	if(strcodetype.CompareNoCase("allcode") == 0)
	{
		propdata.lData =	CODE_FUTURE		|
							CODE_OPTION		|
							CODE_QFUTURE	|
							CODE_QFUTURES	|
							CODE_FSPREAD	|
							CODE_QOPTION	|
							CODE_QOPTIONS	|
							CODE_JOPTION	;
	}
	else
	{
		propdata.lData =	CODE_FUTURE		|
							CODE_OPTION		;
	}

	m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);
	//-------------------------------------------------------------------------<<


	
	//------------------------------------------------------------------------->>
	// �����ڵ��� �ִ� ���� ����
	propdata.lData = 12;
	m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	//2005. 07. 11
	// DR Mode Setting -> �ڵ� ����� �޽����� �˷��ֵ��� �ϱ� ���� 
	propdata.lData	= 1;
	m_pSelCode->SetProperty("SetUserDefMode", &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	// �ʱ� ������ ���� ���õ� �ؿ������ڵ带 �����Ѵ�.
	char szBuf[20];
	int nKind = m_pSelCode->GetDataKind();
	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf))
	{
		if( szBuf[0] )
		{
			m_pSelCode->SetText(szBuf);
		}
	}
	//-------------------------------------------------------------------------<<
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/23
// Return CString		  : �ش� strcode �� ���� �ؿ�������
// Param  CString strcode : �ؿ������ڵ�
// Param  long lcodetype  : �ؿ������ڵ� Ÿ��
// Comments				  :	�ؿ������ڵ忡 ���� �ؿ��������� ��� �´�.
//				- �ؿ������ڵ� �Ӹ� �ƴ϶� �����ڵ� �����ؼ��� ��� �ü� �ִ�.
//-----------------------------------------------------------------------------
CString	CBaseInfo::GetFutOptNameToCodeForDR(CString strcode , long lcodetype /* CODE_EXCH_ALL */)
{
	CString strName;
	
	m_pDBMgr->GetCodeName(lcodetype, strcode, strName);

	return strName;
}

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/29
// Modifier		: 
// Comments		: ���� ���(advise) �� ����(unadvise)
//-----------------------------------------------------------------------------
//#include "new/include/SharedBase.h"
//1. SharedName : �ݵ�� ini�� ��Ͻ�Ų���� ��� �Ѵ�.
//2. pSharedbase : m_hChild -> �ݵ�� �ڵ��� ��ȿ�� �ڵ��� �־��־�� �Ѵ�. ������ ���� �ڵ��̶� �־�ξ�� �Ѵ�.
//                 m_hView -> ������ �����Ҷ� �ʿ��ϴ�. �־ �ǰ� �ȳ־ �ȴ�.
//                 m_hCtrl -> ���⿡�� �ڵ��� ���� �༮�� ���� Unique ID�� ��� �Ѵ�. this ����Ʈ�� ����.
//                 Set�Ҷ� �ڽ��� �ٽ� �ѹ� �� �θ����� �׷��� �ؾ� �ϰ� �����Ҷ��� �ʿ��ϹǷ� �ݵ�� �־�� �Ѵ�.
//3. nSharedType : OR ����.
//#define DE_SHARDSET		1
//#define DE_SHARDGET		2
//#define DE_SHARDSTART	4
//#define DE_SHAREDALL DE_SHARDSET|DE_SHARDGET
//4. nSharedArea
//#define DE_AREA_GENERAL 0  // �Ϲ��� ��Ģ�� ����.
//#define DE_AREA_FREE    1  // ���� ��Ƽ��� ����� ����ũ���� ����(����) 
//#define DE_AREA_ALL		2  // ��ü������ ����(����)	 
//5. nGroup
//DE_AREA_GENERAL�� ��츸 �����ϴ�. Ư���� �׷��ȣ�� �־� ���� �� ��ȣ�θ� ����Ҷ� ��� ����  
BOOL CBaseInfo::AdviseSharedInfo(CString sShareName, CSharedBase* pSharedBase, int nSharedType, int nSharedArea, int nGroup)
{
	return m_pDBMgr->AdviseSharedInfo(sShareName,pSharedBase,nSharedType,nSharedArea,nGroup);
}

BOOL CBaseInfo::UnAdviseSharedInfo(CString sShareName, CSharedBase* pSharedBase)
{
	return m_pDBMgr->UnAdviseSharedInfo(sShareName, pSharedBase);
}

// �Ϲ����� ��� �� ������ ��� �Ѵ�.
//1.hCtrl :  Set�� �� ��� �ڱ�� ���� �ٸ� �༮�����׸� ������ �� ��� ����Ѵ�. Advise���� m_hCtrl�� �� �� �ڵ� �Ǵ� ����ũ����Ʈ
//2.sShareName : ���� �̸�
//3.pData : ���� ������
//4.nLen : ���� ������ ũ��
//5.nSotck : � Ư������ �����Ͷ�� ǥ�� (�ڽ���,�ڽ���, ����,�ɼ� ,,, ���а��� ���� ���� �ȵ�)
//6.bOnlySetData : ���� ����� ���ۿ� �����͸� ���鶧 ���. 
BOOL CBaseInfo::SetSharedData(HWND hCtrl,CString sShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData)
{
	return m_pDBMgr->SetSharedData(hCtrl,sShareName,pData,nLen,nStock,bOnlySetData);
}

// �ַ� ��ũ��Ʈ�� Advise���� �ʴ� ������ ������ ������ ����Ѵ�. ���� ������ �� �ؾ� �Ѵ�.
//1. hArea : ������ ����� �ڵ� (�ݵ�� ���� �� �ִ� ������ �ڵ��̾�� �Ѵ�.)
//2. nArea : �Ʒ� ���������� ������ ����ɼ� �ְ� ���� ����. ���� �ڵ��� �̹����� �ڵ��̾�߸� �Ѵ�. 
//#define AREA_ALL	0
//#define AREA_VIEW	1
//#define AREA_CHILD	2
//#define AREA_GROUP	3
//#define AREA_MAIN	4
//3. ���� ������
//4. ���� ������ ũ��
//5. � Ư������ ������ ǥ��
//6. bOnlySetData : ���� ���ۿ� �����͸� ����.
BOOL CBaseInfo::SetSharedData2(HWND hArea,int nArea, CString sShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData)
{
	return m_pDBMgr->SetSharedData2(hArea,nArea,sShareName,pData,nLen,nStock,bOnlySetData);
}

// return ���� NULL�̸� ���ۿ� �ش� �������� ���� ����. 
BYTE* CBaseInfo::GetSharedData(CString sShareName, int& nLen, int& nStock)
{
	return m_pDBMgr->GetSharedData(sShareName,nLen,nStock);
}

///////////////////////////////////////////////////////////////////////////
// �÷� ���̺� ��� ��� 
COLORREF CBaseInfo::GetColor(int nID)
{
	m_MapColor.m_lClIdx = nID;
	return m_MapColor.GetColor();
}
///////////////////////////////////////////////////////////////////
void CBaseInfo::ShowCodeCombo(int nShow)
{
	m_pSelCode->ShowControl(nShow);
}
void CBaseInfo::SetWindowRectCodeCombo(CRect rect)
{
	m_pSelCode->ReposWindow(rect);
}
void CBaseInfo::EnabelCodeCombo(BOOL bEnable)
{
	HWND hWnd = m_pSelCode->GetCtrlWindow();
	CWnd* cWnd = CWnd::FromHandle(hWnd);
	cWnd->EnableWindow(bEnable);
}

