#ifndef __DFKJDKJFKDJFKDJKFJDK__FKDKFJKDJ_
#define __DFKJDKJFKDJFKDJKFJDK__FKDKFJKDJ_

#include "../../inc/ITreeListMng.h"

class CCallPutItemData
{
public:
	//CString m_szKey;
	CString m_szPrice;
	BOOL	m_isCall;	// call������ True
	BOOL	m_isPut;	// put������ True
	CString m_strCallCode;
	CString m_strPutCode;

public:
	CCallPutItemData()
	{
		m_isCall = FALSE;
		m_isPut = FALSE;
	}
};

typedef struct
{
	char szA[1+1];	// Call/Put����
	char szB[6+1];	// ����
	char szC[10+1];	// ��簡
	char szD[8+1];	// �����ڵ�
	//8+12+30+1
	//201C6227KR4201C62279�ڽ���200 C 200806 227.5      I 
	//201C6227
	//KR4201C62279
	//�ڽ���200 C 200806 227.5      
	//I 
} ST_OPTION_ITEM_DATA;

#define INIT_KOSPI200OPTION_ITEMDATA1(pstVal, xxStr) \
	memset(pstVal, 0x00, sizeof(ST_OPTION_ITEM_DATA));	\
	memcpy(pstVal->szA, &xxStr[6], 1);					\
	memcpy(pstVal->szB, &xxStr[0], 5);					\
	memcpy(pstVal->szC, &xxStr[11], 5);					
#define INIT_KOSPI200OPTION_ITEMDATA2(pstVal, xxStr) \
	memset(pstVal, 0x00, sizeof(ST_OPTION_ITEM_DATA));	\
	memcpy(pstVal->szA, &xxStr[7], 1);					\
	memcpy(pstVal->szB, &xxStr[0], 6);					\
	memcpy(pstVal->szC, &xxStr[12], 5);					


#define INIT_JOPTION_ITEMDATA(pstVal, xxStr) \
	memset(pstVal, 0x00, sizeof(ST_OPTION_ITEM_DATA));	\
	memcpy(pstVal->szA, &xxStr[11], 1);					\
	memcpy(pstVal->szB, &xxStr[13], 6);					\
	memcpy(pstVal->szC, &xxStr[20], 10);
//	memcpy(pstVal->szC, &xxStr[22], 10);

typedef CList<CCallPutItemData*, CCallPutItemData*> List_CCallPutItemData;

class CCallPutIMonthData
{
public:
	CString m_szKey;
	List_CCallPutItemData m_ListData;

public:
	~CCallPutIMonthData()
	{
		ClearList();
	}

	BOOL fnDataToList(ST_OPTION_ITEM_DATA* pstItemData);
	void ClearList();
};

inline void CCallPutIMonthData::ClearList()
{
	CCallPutItemData* pItemData = NULL;
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
		delete m_ListData.GetNext(pos);
	
	m_ListData.RemoveAll();
}

// ������ �����ϴ� ����Ʈ�� �߰��Ѵ�.
inline BOOL CCallPutIMonthData::fnDataToList(ST_OPTION_ITEM_DATA* pstItemData)
{
	// ����Ʈ�� ������ ������Ʈ
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
	{
		CCallPutItemData* pItem = m_ListData.GetNext(pos);
		if(pItem->m_szPrice.Compare(pstItemData->szC)==0)
		{
			if(pstItemData->szA[0]=='C')
			{
				pItem->m_isCall = TRUE;
				pItem->m_strCallCode = pstItemData->szD;
			}
			else if(pstItemData->szA[0]=='P')
			{
				pItem->m_isPut = TRUE;
				pItem->m_strPutCode = pstItemData->szD;
			}
			return TRUE;
		}
	}

	// ���ο� ��簡�̸� �߰��Ѵ�.
	CCallPutItemData* pItem = new CCallPutItemData;
	pItem->m_szPrice = pstItemData->szC;

	if(pstItemData->szA[0]=='C')
	{
		pItem->m_isCall = TRUE;
		pItem->m_strCallCode = pstItemData->szD;
	}
	else if(pstItemData->szA[0]=='P')
	{
		pItem->m_isPut = TRUE;
		pItem->m_strPutCode = pstItemData->szD;
	}

	m_ListData.AddTail(pItem);

	return TRUE;
}

typedef CList<CCallPutIMonthData*, CCallPutIMonthData*> List_CCallPutIMonthData;

class CCallPutIHelper
{
public:
	List_CCallPutIMonthData m_ListData;

	CCallPutIMonthData* m_pCurMontList;

	void Parsing(LIST_CCodeTypeA* pCodeType, int nCodeType = 0);	// 1 : kospi200�ɼ�, 2 : �ֽĿɼ�, 0 : �׿�
	void ClearList();
	CCallPutIMonthData* FindItemData(ST_OPTION_ITEM_DATA* pstItemData);
	CCallPutIMonthData* FindItemData(LPCSTR szMonth);
	BOOL fnDataToList(ST_OPTION_ITEM_DATA* pstItemData);

public:
	CCallPutIHelper()
	{
		m_pCurMontList = NULL;
	}

	~CCallPutIHelper()
	{
		ClearList();
	}
};



inline void CCallPutIHelper::Parsing(LIST_CCodeTypeA* pCodeType, int nCodeType /*= 0*/)
{
	if(nCodeType == 0)
		return;

	CCodeTypeA typeA;
	CString szOpName ;
	ST_OPTION_ITEM_DATA stItemData;
	ST_OPTION_ITEM_DATA* pstItemData = &stItemData;
	char* pszOpName;
	CString strOptName;
	int nPosOptName = -1;
	CString strCallPut;
	int nPosCallPut = -1;
	for(POSITION pos = pCodeType->GetHeadPosition(); pos;)
	{
		typeA = pCodeType->GetNext(pos);
		szOpName = typeA.m_szhname;
		pszOpName = (LPSTR)(LPCSTR)szOpName;
		strOptName = (LPSTR)(LPCSTR)szOpName;
		nPosOptName = strOptName.Find("����");
		nPosCallPut = strOptName.Find("Call");
		if(nCodeType == 1)			// kospi200�ɼ�
		{
// @Solomon-->
			memset(pstItemData, 0x00, sizeof(ST_OPTION_ITEM_DATA));
			memcpy(pstItemData, strOptName, strOptName.GetLength());
			memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
			pstItemData->szA[ 1] = 0;
			pstItemData->szB[ 6] = 0;
			pstItemData->szC[10] = 0;

// 				if(nPosOptName == 1)
// 				{
// 	//				INIT_KOSPI200OPTION_ITEMDATA1(pstItemData, pszOpName);
// 					memset(pstItemData, 0x00, sizeof(ST_OPTION_ITEM_DATA));
// 					if(nPosCallPut > 0)	// call
// 					{
// 						memcpy(pstItemData->szA, &pszOpName[6], 1);
// 						memcpy(pstItemData->szB, &pszOpName[0], 5);
// 						memcpy(pstItemData->szC, &pszOpName[11], 5);					
// 					}
// 					else				// put
// 					{
// 						memcpy(pstItemData->szA, &pszOpName[6], 1);
// 						memcpy(pstItemData->szB, &pszOpName[0], 5);
// 						memcpy(pstItemData->szC, &pszOpName[10], 5);					
// 					}
// 					memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
// 				}
// 				else if(nPosOptName == 2)
// 				{
// 	//				INIT_KOSPI200OPTION_ITEMDATA2(pstItemData, pszOpName);
// 					memset(pstItemData, 0x00, sizeof(ST_OPTION_ITEM_DATA));
// 					if(nPosCallPut > 0)	// call
// 					{
// 						memcpy(pstItemData->szA, &pszOpName[7], 1);
// 						memcpy(pstItemData->szB, &pszOpName[0], 6);
// 						memcpy(pstItemData->szC, &pszOpName[12], 5);					
// 					}
// 					else				// put
// 					{
// 						memcpy(pstItemData->szA, &pszOpName[7], 1);
// 						memcpy(pstItemData->szB, &pszOpName[0], 6);
// 						memcpy(pstItemData->szC, &pszOpName[11], 5);					
// 					}
// 					memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
// 				}
// 	// @Solomon<--
		}
		else if(nCodeType == 2)		// �ֽĿɼ�
		{
			INIT_JOPTION_ITEMDATA(pstItemData, pszOpName);
			memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
		}
		else if(nCodeType == 3)		// �޷��ɼ�
		{
			INIT_JOPTION_ITEMDATA(pstItemData, pszOpName);
			memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
		}

		fnDataToList(pstItemData);
	}
}


inline BOOL CCallPutIHelper::fnDataToList(ST_OPTION_ITEM_DATA* pstItemData)
{
	if( m_pCurMontList && m_pCurMontList->m_szKey.Compare(pstItemData->szB)==0)
	{
		return m_pCurMontList->fnDataToList(pstItemData);
	}

	CCallPutIMonthData* pItemData = FindItemData(pstItemData);
	if(pItemData)
	{
		m_pCurMontList = pItemData;
		return m_pCurMontList->fnDataToList(pstItemData);
	}
	return FALSE;
}

// �����ϴ� ����Ʈ������ ã�´�.
inline CCallPutIMonthData* CCallPutIHelper::FindItemData(LPCSTR szMonth)
{
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
	{
		CCallPutIMonthData* pItem = m_ListData.GetNext(pos);
		if( pItem->m_szKey.Compare(szMonth)==0)
		{
			return pItem;
		}
	}
	return NULL;
}

// ����Ʈ�� ������ ���ο� ������ �����Ѵ�.
inline CCallPutIMonthData* CCallPutIHelper::FindItemData(ST_OPTION_ITEM_DATA* pstItemData)
{
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
	{
		CCallPutIMonthData* pItem = m_ListData.GetNext(pos);
		if( pItem->m_szKey.Compare(pstItemData->szB)==0)
		{
			return pItem;
		}
	}

	CCallPutIMonthData* pItem = new CCallPutIMonthData;
	pItem->m_szKey = pstItemData->szB;

	m_ListData.AddTail(pItem);
	return pItem;
}

inline void CCallPutIHelper::ClearList()
{
	CCallPutIMonthData* pMonthData = NULL;
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
		delete m_ListData.GetNext(pos);

	m_ListData.RemoveAll();
	m_pCurMontList = NULL;
}

#endif //__DFKJDKJFKDJFKDJKFJDK__FKDKFJKDJ_
