#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"

// ȭ�� ��������Ÿ�� ����ϱ� ���� �ʱ��ƾ
// GET �� ���õǾ� ������ ���� �о�´�.
// REAL �� ���õǾ� ������ �������� ����Ѵ�.
BOOL CExOCXDLL::InitLSharedData()
{
	if(!m_pBaseDesk)	return FALSE;
	HWND hWnd = (HWND)DGetBaseDeskHWnd();
	if(!hWnd && !IsWindow(hWnd)) return FALSE;

	ILSharedData* pLSharedData = (ILSharedData*)m_pBaseDesk->GetLSharedData();
	
	if(pLSharedData == NULL) 
	{
		TRACE("ERROR : CExOCXDLL::InitLSharedData()\n");
		return FALSE;
	}

	m_pILSharedData = pLSharedData;

	CString szData = m_sLocalSharedList;
	if(szData.GetLength()==0)	return TRUE;

	szData += ";";
	int nIndex = szData.Find(";");
	CString szShareName;
	BOOL	bGet, bSet, bStart;
	char	aTmp[2]={0,};
	int		nLen, nCnt=0, nTmp;

	while(nIndex>0)
	{
		bGet = bSet = bStart = FALSE;
		szShareName = szData.Left(nIndex);
		szData = szData.Mid(nIndex+1);

		nLen = szShareName.GetLength();
		aTmp[0] = szShareName[nLen-1];
		szShareName = szShareName.Left(nLen-1);

		nTmp = atoi(aTmp);
		if(nTmp & 1) bStart = TRUE;
		if(nTmp & 2) bSet=TRUE;
		if(nTmp & 4) bGet=TRUE;

		ST_LSHAREDFORMAT LSDF;
		LSDF.m_bGet = bGet;
		LSDF.m_bSet = bSet;
		LSDF.m_bStart = bStart;
		LSDF.m_szSharedName = szShareName;
		m_ListLShare.AddTail(LSDF);

		if(bStart) 
		{
			CString szValue;
			InitStart(szShareName, szValue);
		}

		if(bSet) 
		{
		
		}

		if(bGet) 
		{
			InitGet(szShareName);
		}

		nCnt++;

		nIndex = szData.Find(";");
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////
//Start
//Map Loading�� ���� Data�� ������...
BOOL CExOCXDLL::InitStart(LPCSTR szShareName, CString& szValue)
{	
	if(m_pILSharedData)
	{
		BOOL bRet = m_pILSharedData->GetData(szShareName, szValue);
		if(!bRet)
			szValue.Empty();

		OnLSharedDataChange(szShareName, szValue, TRUE);
//		ParedShareDataChange(szShareName,szValue);		//Added by KaySeo, 2000/11/22
	
		return TRUE;
	}
	return FALSE;
}

BOOL CExOCXDLL::InitSet()
{
	return TRUE;
}	
	
// ������ ���� Data�� �����Ѵ�.
// ��..���� �ڵ尡 �ٲ�� �ٲ� ���� �ڵ带 �޴´�.
BOOL CExOCXDLL::InitGet(LPCSTR szShareName)
{	
//	if(m_pILSharedData)
//	{
//		m_pILSharedData->DAdvise(&m_xLAdviseSink, szShareName);
//		return m_pILSharedData->DAdvise(&m_xLAdviseSink, szShareName);
//	}
	return FALSE;
}	
	
	
BOOL CExOCXDLL::ChangedLShareData(LPCSTR szShareName, LPCSTR szValue)
{	
	if(m_pILSharedData)
		return m_pILSharedData->SetData(m_hWnd, szShareName, szValue);
	return FALSE;
}	

BOOL CExOCXDLL::ParedShareDataChange(LPCSTR szSharedName, LPCSTR szData)
{
//	TRACE("CHtcInputExCtrl::ParedShareDataChange ==> ShareName[%s] Value[%s]\n", szSharedName, szData);

	ST_LSHAREDFORMAT LSDF;
	POSITION tmpPos;
	for(POSITION pos=m_ListLShare.GetHeadPosition(); pos; )
	{
		tmpPos = pos;
		LSDF = m_ListLShare.GetNext(pos);
		if(LSDF.m_bGet || LSDF.m_bStart)
		{
			if(LSDF.m_szSharedName==szSharedName)
			{
				LSDF.m_szData = szData;		//Added by Kay Seo, 2000/11/22

				TRACE("Output List --> SharedName[%s],Value[%s]\n", LSDF.m_szSharedName, LSDF.m_szData);
				
				m_ListLShare.SetAt(tmpPos,LSDF);

				OnLSharedDataChange(szSharedName, szData, FALSE);
			}
		}
	}

	return TRUE;
}
//
//
//void CExOCXDLL::NeoCheckLSharedData(LPCTSTR szOldData, LPCTSTR szNewData)
//{
////	if(!m_bControlActive)
////		return;
//	ST_LSHAREDFORMAT LSDF;
//	for(POSITION pos=m_ListLShare.GetHeadPosition(); pos; )
//	{
//		LSDF = m_ListLShare.GetNext(pos);
//
//		if(LSDF.m_bSet)
//			ChangedLShareData(LSDF.m_szSharedName, szNewData);	
//	}
//}
//
//long CExOCXDLL::FV_lRmsgLSharedChaned(WPARAM wParam, LPARAM lParam)
//{
//	LPST_LSHAREDATA pSHD = (LPST_LSHAREDATA)wParam;
//	if(pSHD)
//	{
//		CString szShareName = pSHD->m_szShareName;
//		CString szData;
//		
//		ST_LSHAREDFORMAT LSDF;
//		POSITION tmpPos;
//		for(POSITION pos=m_ListLShare.GetHeadPosition(); pos; )
//		{
//			tmpPos = pos;
//			LSDF = m_ListLShare.GetNext(pos);
//			if(LSDF.m_bGet)
//			{
//				if(LSDF.m_szSharedName==szShareName)
//				{
//					m_bLSharedDataRes=TRUE;
//					LSDF.m_szData = szData;		//Added by Kay Seo, 2000/11/22
//					
//					TRACE("Output List --> SharedName[%s],Value[%s]\n", LSDF.m_szSharedName, LSDF.m_szData);
//					
//					m_CodeEdit.SetWindowText(pSHD->m_szShareData);				
//					m_bLSharedDataRes=FALSE;
////					FT_vInvokeShare(pSHD->m_szShareData);
//					NeoFireLengthFull(pSHD->m_szShareData);
//					// �ڼ��� 0329. �����߻��� �ڵ屸���� output���� �����ֱ� ����
//					SetTextToOtherControls();
//
//					NeoFireSendTr(pSHD->m_szShareData);	
//				}
//			}
//		}
//		
//		return 0L;
//	}
//	
//	return 0L;
//}
