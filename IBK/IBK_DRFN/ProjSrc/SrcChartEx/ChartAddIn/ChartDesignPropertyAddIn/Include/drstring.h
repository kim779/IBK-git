class CDrString
{
public:
	//�ڿ����� �˻� ���� 0x20���� ũ�� NULL�� �ְ� ������.
	static void TrimRight(CString& str)
	{
		LPSTR lpsz = (LPSTR)(LPCSTR)str;
		int len = strlen(lpsz);
		for(int k=len-1;k>-1;k--)
		{
			if ((UINT)lpsz[k] <= 0x20) continue;
			else 
			{
				if(len == k+1) return;
				str.GetBufferSetLength(k+1);
				return;
			}
		}
	}
	//�ڿ����� �˻� ���� 0x20���� ũ�� NULL�� �ְ� ������.
	static void TrimRight(char* pStr)
	{
		int len = strlen(pStr);
		for(int k=len;k>-1;k--)
		{
			if ((UINT)pStr[k] <= 0x20) continue;
			else 
			{
				pStr[k+1]=NULL;
				return;
			}
		}
	}

	static void TrimLeft(CString& str)
	{
		int len = str.GetLength();
		LPTSTR lpsz = (LPSTR)(LPCSTR)str;
		for(int k=0;k<len;k++)
		{
			if ((UINT)lpsz[k] <= 0x20) continue;
			else 
			{
				if(k==0) return;
				memcpy(lpsz,lpsz+k,len-k);
				str.GetBufferSetLength(len-k);
				return;
			}
		}

	}	

	static void TrimLeft(char* pStr)
	{
		int len = strlen(pStr);
		for(int k=0;k<len;k++)
		{
			if ((UINT)pStr[k] < 0x21) continue;
			else
			{
				if(k==len) return;
				if(k>=0){
					memcpy(pStr,pStr+k,len-k);
					pStr[len-k]=NULL;
					return;
				}
			}
		}
	}

	static int Mid(CString& baseStr, CString& sMid, int nStart, int nLast=0)
	{
		char* pTempStr = (LPSTR)(LPCSTR)baseStr;
		if(!pTempStr) return -1;
		
		if(nLast==0)
		{
			nLast = baseStr.GetLength();
			nLast = nLast - nStart;
		}
		else
		{
			if(nLast >= baseStr.GetLength()) return -1;
		}

		if(nLast < 0 ) return -1;
		
		LPTSTR pMid = sMid.GetBufferSetLength(nLast);
		memcpy(pMid,pTempStr+nStart,nLast);
		
		return nLast;
	}

	static int Mid(CString& baseStr, char* pMid, int nStart, int nLast=0)
	{
		char* pTempStr = (LPSTR)(LPCSTR)baseStr;
		if(!pTempStr) return -1;
		
		if(nLast==0) {
			nLast = baseStr.GetLength();
			nLast = nLast - nStart;
		}
		else
		{
			if(nLast >= baseStr.GetLength()) return -1;
		}

		if(nLast < 0 ) return -1;
		
		memcpy(pMid,pTempStr+nStart,nLast);
		pMid[nLast]=NULL;
		
		return nLast;
	}

	static int Mid(char* baseStr, char* pMid, int nStart, int nLast=0)
	{
		if(baseStr) return -1;
		
		if(nLast==0) {
			nLast = strlen(baseStr);
			nLast = nLast - nStart;
		}
		else
		{
			if(nLast >= (int)strlen(baseStr)) return -1;
		}
		
		if(nLast < 0 ) return -1;
		
		memcpy(pMid,baseStr+nStart,nLast);
		pMid[nLast]=NULL;
		
		return nLast;
	}

	static int Right(CString& baseStr, CString& sRihgt, int nCnt)
	{
		char* pTempStr = (LPSTR)(LPCSTR)baseStr;
		if(!pTempStr) return -1;
		
		int nLast = baseStr.GetLength();
		if(nLast <= nCnt) return -1;
		
		LPTSTR pMid = sRihgt.GetBufferSetLength(nCnt);
		memcpy(pMid,pTempStr+nLast-nCnt,nCnt);
		
		return nCnt;
	}

	static int Right(char* baseStr, char* pMid, int nCnt)
	{
		if(!baseStr) return -1;
		
		int nLast = strlen(baseStr);
		if(nLast <= nCnt) return -1;
		
		memcpy(pMid,baseStr+nLast-nCnt,nCnt);
		pMid[nCnt]=NULL;
		
		return nCnt;
	}

	static int Right(CString& baseStr, char* pMid, int nCnt)
	{
		char* pTempStr = (LPSTR)(LPCSTR)baseStr;
		if(!pTempStr) return -1;
		
		int nLast = baseStr.GetLength();
		if(nLast <= nCnt) return -1;
		
		memcpy(pMid,pTempStr+nLast-nCnt,nCnt);
		pMid[nCnt]=NULL;
		
		return nCnt;
	}


	static int Left(CString& baseStr, char* pLeft, int nIndex)
	{
		char* pTempStr = (LPSTR)(LPCSTR)baseStr;
		if(!pTempStr) return -1;
		
		int nLast = baseStr.GetLength();
		if(nLast <= nIndex) return -1;

		memcpy(pLeft,pTempStr,nIndex);
		pLeft[nIndex+1]=NULL;
		
		return nIndex+1;
	}


	static int Left(char* baseStr, char* pLeft, int nIndex)
	{
		if(!baseStr) return -1;
		
		int nLast = strlen(baseStr);
		if(nLast <= nIndex) return -1;
		
//		memcpy(pLeft,baseStr,nIndex-1);
//		pLeft[nIndex]=NULL;

		memcpy(pLeft,baseStr,nIndex);
		pLeft[nIndex+1]=NULL;
		
		return nIndex+1;
	}

	static int Left(CString& baseStr, CString& strMid, int nIndex)
	{
		char* pTempStr = (LPSTR)(LPCSTR)baseStr;
		if(!pTempStr) return -1;
		
		int nLast = baseStr.GetLength();
		if(nLast <= nIndex) return -1;

//		LPTSTR pLeft = strMid.GetBufferSetLength(nIndex);
//		memcpy(pLeft,pTempStr,nIndex-1);
//		pLeft[nIndex]=NULL;

		// Left�� ���� index���� ���� �������Ƿ�, ���� ���̸� 1��ŭ �߰�..
		LPTSTR pLeft = strMid.GetBufferSetLength(nIndex);
		memcpy(pLeft,pTempStr,nIndex);
		// GetBufferSetLength�� �������� NULL �߰� ���ʿ�.. ���ο��� �ڵ� �ذ�
		
		return nIndex+1;
	}
};
//====================================================================================================
// End Of CDrString
//====================================================================================================

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*
class CParsingString
{
public:
	CParsingString(CString& sData)
	{
		m_start = 0;
		m_end = 0;
		m_pToken = NULL;
		m_pMainStr = (LPSTR)(LPCSTR)sData;
	}
public:
	int m_start;
	int m_end;

public:
	LPSTR m_pMainStr;
	LPSTR m_pToken;

public:
	CParsingString();
	LPSTR GetNext(int nch, int& len);
	LPSTR GetData(){return m_pMainStr;};
	int   GetLength(){return (m_end-m_start-1);};
	void  MakeInfoMapString(char* info, int len, CMapStringToString *infoMap, int nChar=0);
};

char* CParsingString::GetNext(int nCh, int& len)
{
	if(m_pMainStr==NULL) return NULL;
	char* token = strchr(m_pMainStr+m_start,nCh);	
	
	if(token == NULL) return NULL;
	m_pToken = m_pMainStr + m_start;

	m_end = token - m_pMainStr +1;		
	len = m_end-m_start-1;
	m_start = m_end;
	return m_pToken;
}
*/

//*********************************************************************************/ 
// �Լ���    : MakeInfoMapString(char* info, int len, CMapStringToString *infoMap)  
// Returned : None
// �Ķ����  :  *info :  �Ľ��� ���ڿ� ������ CMapStringToString pointer
//				len : ���ڿ� ����
//				MapStringToString : infoMap	
// �Լ� ���� : [item=data|item=data||||] �Ľ��� CString�� �����ʰ� �ϴ� ���
// ��������  : 
// �ۼ���    : ������ 
//*********************************************************************************/
/*
#define _TEMP_BUFFER 64
#define _TEMP_MAXBUFFER 4096

void CParsingString::MakeInfoMapString(char* info, int len, CMapStringToString *infoMap, int nChar) 
{
	static char makeBuff[_TEMP_MAXBUFFER];

	if(_TEMP_MAXBUFFER < len) {
		AfxMessageBox("CParsingString::MakeInfoMapString : buffer size error");
		return;
	}
	infoMap->RemoveAll(); 
	
	char seperator[] = "|";
	char *token;
	int  index = 0;
	
	char tempBuff1[_TEMP_BUFFER];
	char tempBuff2[_TEMP_BUFFER];
	char* ptemp;
	if(nChar) //���� �κ� ����
	{
		//int stFind ='[';
		ptemp = strchr(info,nChar);
		if(ptemp == NULL) return;
		index = ptemp - info +1;
	}

	memset(makeBuff,0x00,len+1);
	memcpy(makeBuff,info+index,len-1);
	token = strtok(makeBuff, seperator);
	
	int ch = '=';
	while(token != NULL)   {
		memset(tempBuff1,0x00,_TEMP_BUFFER);
		memset(tempBuff2,0x00,_TEMP_BUFFER);
		ptemp = strchr(token,ch);  
		if(ptemp)
		{
			index = ptemp - token ;
			memcpy(tempBuff1,token,index);
			memcpy(tempBuff2,token+(index+1),strlen(token)-index);
		}
		infoMap->SetAt(tempBuff1, tempBuff2);
		token = strtok(NULL, seperator);
	}
}
*/

/*
void CTableInfo::SetCellPropInfo(CString info)
{
	CMapStringToString mapString;

	CString str, Row, Col;

	//{{������ 2.6
	int stEnd = ']';
	int stStart = '[';
	char* token;
	CParsingString parser(info);
	int len;
	while( (token = parser.GetNext(stEnd,len)) ){
	//}}������

		parser.MakeInfoMapString(token,len, &mapString,stStart);
		//}}
		mapString.Lookup("Row", Row);
		mapString.Lookup("Col", Col);
		SetCellPropInfo(CCellID(atoi(Row), atoi(Col)), &mapString);
	}

	int Cnt = mapString.GetCount();
	CCellInfo* pCell = NULL;
	m_pOutputList.RemoveAll(); // output type �� ���� ������ list (2002.07.03 ������)
	for(int row = 0; row < m_nRows; row++)	
	{
		for(int col = 0; col < m_nCols; col++) 
		{
			pCell = (CCellInfo *)GetCell(row, col);
			if(!pCell) continue;
			if(pCell->GetCellType() != 1) continue;
			m_pOutputList.AddTail(pCell);
		}
	}
}
*/