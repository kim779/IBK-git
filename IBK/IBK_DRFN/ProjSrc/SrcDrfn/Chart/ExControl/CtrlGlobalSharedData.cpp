#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"

// (2004.06.08, ��¿�) Global ���� ����� �߰��Ѵ�. (Input OCX���� Porting��)
//////////////////////////////////////////////////////////////////////////////////
// Global ����������
// Global �Ҷ� �Ʒ� �� �κ��� �����ؼ� ����ϼ���.
// 2000. 11. 29 ���ؿ�(JunOk Lee)  alzio@dooriic.co.kr

BOOL CExOCXDLL::CGSharedCli::ChangedData(LPCSTR _szName, LPCSTR _szValue)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)
		
	POSITION pos = FindShareData( _szName);
	if(pos)
	{
		TRACE("CGSharedCli::ChangedData [SHAREDNAME=%s] [VALUE=%s]\n",_szName,_szValue);
		// ���������Ͱ� �ٲ����.
		// ���⿡�� ���ϴ� ó���� �Ѵ�.
		if( m_pGSharedSvr)
		{
			m_bGetGData = TRUE;

#ifdef _DEBUG
			CString szTest;
			szTest.Format("Get ���� Data ---> ������ : %s, Data : %s",_szName, _szValue);
			TRACE(szTest);
//			AfxMessageBox(szTest);
#endif

			m_szGSharedData = _szValue;
			m_szGSharedData.TrimLeft();
			m_szGSharedData.TrimRight();
			if( m_szGSharedData.GetLength() == 0) m_bGetGData = FALSE;

			pThis->OnGSharedDataChange( _szName, m_szGSharedData, FALSE);

			// �ڼ��� 2002.08.09
			// ������ ������ ���� FALSE�� ����� ����Ѵ�.
			m_bGetGData = FALSE;
		}
	}

	return TRUE;
}

BOOL CExOCXDLL::CGSharedCli::GetData(LPCSTR _szName, CString& szValue)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->GetData(_szName, szValue);

	return FALSE;
}

BOOL CExOCXDLL::CGSharedCli::SetData(LPCSTR _szName, LPCSTR _szValue)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->SetData(_szName, _szValue, this);
	return FALSE;
}

BOOL CExOCXDLL::CGSharedCli::Advise(LPCSTR _szName, CGlobalSharedClient* _pClient)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->Advise(_szName, this);
	return FALSE;
}

BOOL CExOCXDLL::CGSharedCli::UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->UnAdvise(_szName, this);
	return FALSE;
}

void CExOCXDLL::CGSharedCli::MakeList(CString szList)
{
	m_ListShare.RemoveAll();

	if(szList.GetLength()==0)		return;

	CString szData = szList;
	szData += ";";
	int nIndex = szData.Find(";");
	CString szShareName;

	BOOL	bStart, bSet, bGet;
	char	aTmp[2]={0,};
	int		nLen, nCnt=0, nTmp;

	while(nIndex>0)
	{
		bStart = bSet = bGet = FALSE;
		szShareName = szData.Left(nIndex);
		szData = szData.Mid(nIndex+1);

		nLen = szShareName.GetLength();
		aTmp[0] = szShareName[nLen-1];
		szShareName = szShareName.Left(nLen-1);

		nTmp = atoi(aTmp);
		if(nTmp & 1) bStart = TRUE;
		if(nTmp & 2) bSet=TRUE;
		if(nTmp & 4) bGet=TRUE;

		ST_GSHAREDFORMAT SDF;
		SDF.m_bStart = bStart;
		SDF.m_bSet = bSet;
		SDF.m_bGet = bGet;
		SDF.m_szSharedName = szShareName;
		m_ListShare.AddTail(SDF);

		nIndex = szData.Find(";");
	}
}

POSITION CExOCXDLL::CGSharedCli::FindShareData(LPCSTR _szName)
{
	CString szShareList = _szName;
	szShareList += ";";
	CString szTmp;

	ST_GSHAREDFORMAT SDF;
	POSITION tmpPos;
	for(POSITION pos=m_ListShare.GetHeadPosition(); pos; )
	{
		tmpPos=pos;
		SDF = m_ListShare.GetNext(pos);
		if(SDF.m_bGet)
		{
			szTmp.Format("%s;", SDF.m_szSharedName);
			szTmp.Remove(';');
			szShareList.Remove(';');
			if(szShareList.CompareNoCase(szTmp) == 0)
				return tmpPos;
		}
	}

	return NULL;
}

void CExOCXDLL::CGSharedCli::CallCheckSharedData(LPCSTR _szData)
{
	if(m_bGetGData == TRUE) 
	{
		m_bGetGData = FALSE;
		TRACE("������ ���ؼ� ���Ա� ���� Set���� �߻� ����..[%s]\n",_szData);
		return;
	}

	TRACE("Set���� �߻� [%s]\n",_szData);
	ST_GSHAREDFORMAT SDF;
	CString szShareNameList;
	for(POSITION pos=m_ListShare.GetHeadPosition(); pos; )
	{
		SDF = m_ListShare.GetNext(pos);
		if(SDF.m_bSet)
		{
			szShareNameList += SDF.m_szSharedName;
			szShareNameList += ";";
		}
	}

	if(szShareNameList.GetLength() > 0)
		SetData(szShareNameList, _szData);
}

int  CExOCXDLL::CGSharedCli::AddSharedNameToServer()
{
	// ����� Ŭ���̾�Ʈ �����͸� ����Ѵ�.
	// ������ �߻��� ��� ������ �´�.
	if(!m_pGSharedSvr) return 0;

	m_pGSharedSvr->Advise("*", this);

	int nRet=1;
	return nRet;
}

int  CExOCXDLL::CGSharedCli::RemoveSharedNameToServer()
{
	// ����� Ŭ���̾�Ʈ �����͸� ����Ѵ�.
	// ������ �߻��� ��� ������ �´�.
	if(!m_pGSharedSvr) return 0;

	m_pGSharedSvr->UnAdvise("*", this);

	int nRet=1;
	return nRet;
}

void CExOCXDLL::CGSharedCli::IniStartGShareData()
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)


	ST_GSHAREDFORMAT SDF;

	for(POSITION pos=m_ListShare.GetHeadPosition(); pos; )
	{
		SDF = m_ListShare.GetNext(pos);
		if(SDF.m_bStart)
		{
			m_bGetGData =TRUE;
			m_szGSharedData = GetData2(SDF.m_szSharedName);
#ifdef _DEBUG
			CString szTest;
			szTest.Format("Start ���� Data ---> ������ :[%s], Data :[%s]\n",SDF.m_szSharedName, m_szGSharedData);
			TRACE(szTest);
#endif
			m_szGSharedData.TrimLeft();
			m_szGSharedData.TrimRight();

			if(m_szGSharedData.GetLength() <= 0) 
				m_bGetGData = FALSE;

			pThis->OnGSharedDataChange( SDF.m_szSharedName, m_szGSharedData, TRUE);

			// �ڼ��� 2002.08.09
			// ������ ������ ���� FALSE�� ����� ����Ѵ�.
			m_bGetGData = FALSE;
		}
	}

}

LPCSTR CExOCXDLL::CGSharedCli::GetData2(LPCSTR _szName)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->GetData2(_szName);
	return "";
}


// Global ����������
//////////////////////////////////////////////////////////////////////////////////

