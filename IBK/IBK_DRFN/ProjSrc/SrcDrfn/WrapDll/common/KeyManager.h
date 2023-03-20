// SetManager.h: interface for the CSetManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_KEYMANAGER_H__D3798E30_573E_4a2b_9BBC_848D9D6A6C72__INCLUDED_)
#define AFX_KEYMANAGER_H__D3798E30_573E_4a2b_9BBC_848D9D6A6C72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TR_BASEDATA			1
#define TR_KP200			2
#define TR_KOSPI			3
#define TR_REMAIN			4	// �ܰ���ȸ
#define TR_AVERAGEIV		5	// ��ճ��纯����
#define TR_MARGININFO		6	// �������ű�����
#define TR_THEORY4MARGIN	7	// ���űݿ� �̷а�
#define TR_PRICEPROGRESS	8	// ��������
#define TR_AVERAGETODAYIV	9	// ����պ�����
#define	TR_OPTION_MONTH		10	// �ɼǿ����� ��簡
#define	TR_SOPTION_MONTH	11	// �ֽĿɼǿ����� ��簡
#define	TR_DELTA_HEDGE_INFO	12	// ��Ÿ���� ����
//20080228 �̹��� >>
#define	TR_TOTALIV			13 //����� ���纯����
#define	TR_HIS_IV_TOT_IV_DATE	14//���ں� ������, ����ճ��� ������
//20080228 �̹��� <<

class CKeyInfo
{
public:
	CKeyInfo()
	{
		m_nRef = 0;
		m_nKey = -1;
	}

	int m_nRef;
	int m_nKey;
};

class CKeyManager  
{
private:
	CUIntArray m_KeyArray;
	CMapStringToPtr m_MapCommInfo;
	
	int GetNextKey()
	{
		if(m_KeyArray.GetSize() == 0) return -1;
		int nKey = m_KeyArray.GetAt(0);
		m_KeyArray.RemoveAt(0);
		return nKey;
	}

public:
	CKeyManager(UINT nStartKey = 50, UINT nEndKey = 255)
	{
		for(UINT i = nStartKey; i < nEndKey; i++)
			m_KeyArray.Add(i);
	}


	~CKeyManager()
	{
		ResetKey();
	}

	void ResetKey()
	{
	    CString  strKey;
		CKeyInfo* pInfo;

		POSITION pos = m_MapCommInfo.GetStartPosition();
		while( pos != NULL )
		{
			m_MapCommInfo.GetNextAssoc( pos, strKey, (void*& )pInfo );

			if(pInfo) delete pInfo;

			// Ű�� �����Ѵ�.
			m_MapCommInfo.RemoveKey( strKey );
		}

		m_KeyArray.RemoveAll();
		for(UINT i = 50; i < 255; i++)
			m_KeyArray.Add(i);
	}


	BOOL IsExistCode(CString strCode)
	{
		CKeyInfo* pInfo;
		if(m_MapCommInfo.Lookup(strCode, (void*&)pInfo))
			return TRUE;

		return FALSE;
	}

	void SendBackKey(CString strCode)
	{
		CKeyInfo* pInfo = NULL;
		if(m_MapCommInfo.Lookup(strCode, (void*&)pInfo))
		{
			pInfo->m_nRef--;
			
			if(pInfo->m_nRef == 0)
			{
				m_KeyArray.Add(pInfo->m_nKey);
				
				CString strKey;
				POSITION pos = m_MapCommInfo.GetStartPosition();
				while( pos != NULL )
				{
					m_MapCommInfo.GetNextAssoc( pos, strKey, (void*& )pInfo );

					if(strCode == strKey)
					{
						if(pInfo){
							delete pInfo;
							pInfo = NULL;
						}

						// Ű�� �����Ѵ�.
						m_MapCommInfo.RemoveKey( strKey );

						break;
					}
				}// end of while
			}// end of if
		}
	}
	int	GetKey(CString strCode)
	{
		CKeyInfo* pInfo;
		if(m_MapCommInfo.Lookup(strCode, (void*&)pInfo))
		{
			pInfo->m_nRef++;
			return pInfo->m_nKey;
		}
		else
		{
			int nKey = GetNextKey();
			if(nKey != -1)
			{
				pInfo = new CKeyInfo();
				pInfo->m_nKey = nKey;
				pInfo->m_nRef = 1;
				m_MapCommInfo[strCode] = pInfo;
			}
			return nKey;
		}
	}
};




#endif // !defined(AFX_KEYMANAGER_H__D3798E30_573E_4a2b_9BBC_848D9D6A6C72__INCLUDED_)
