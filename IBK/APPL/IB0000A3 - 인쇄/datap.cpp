#include "stdafx.h"
#include "datap.h"


#define			FORMAT_TEXT					0
#define			FORMAT_NUMCOMMA_SIGN		1
#define			FORMAT_NUMCOMMA				2
#define			FORMAT_ACCOUNT				3
#define			FORMAT_YYMMDD				4
#define			FORMAT_MMDD					5
#define			FORMAT_HHMMSS				6
#define			FORMAT_MMSS					7
#define			FORMAT_ONLYINT				8


///////////////////////////////////////////////////////////////////////////////////////////////////
CSize GetTextSize(CDC* pDC, CString sText)
{
	const int ispace = 2*2;
	CSize size = pDC->GetTextExtent(sText); 
	size.cx += ispace;
	return size;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL FileExist(CString sPath)
{
	CFileStatus fs;
	return CFile::GetStatus(sPath, fs); 

}
///////////////////////////////////////////////////////////////////////////////////////////////////
CString ResourceIDLoadString(UINT nID)
{
	CString strResult;
	strResult.LoadString(nID);
	return strResult;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CString MakeString(CString sData, CString sFillToken, int ilen)
{
	for(int i=sData.GetLength(); i<ilen; i++)
		sData += sFillToken;

	return sData;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CString RemoveSign(CString sData, CString sign1, CString sign2)
{
	sData.Replace(sign1, "");
	if(sign2 != "")
		sData.Replace(sign2, "");

	return sData;

}
bool isNum(char c)
{
	char *pnum = "0123456789.";
	
	for(int i=0; i< int(strlen(pnum)) ; i++)
	{
		if(c == pnum[i]) return true;
	}


	return false;
}

BOOL isNumber(CString sData)
{
	if(sData.FindOneOf("+-") == 0)
		sData = sData.Right(sData.GetLength() -1);  

	sData = Trim(sData);
	if(sData.IsEmpty() ) return FALSE;
	
	for(int i=0; i<sData.GetLength(); i++)
	{
		if( !isNum( sData.GetAt(i) ))
			return FALSE;
	}

	return TRUE;
}

CString RemoveText(CString sData)
{
	CString sResult = "";
	for(int i=0; i<sData.GetLength(); i++)
	{
		if( isNum( sData.GetAt(i) )) sResult += sData.GetAt(i) ;
	}

	return sResult;
}
CString RemoveToken(CString sData, char cToken)
{
	CString sResult = "";
	for(int i=0; i<sData.GetLength(); i++)
	{
		if(sData.GetAt(i) !=  cToken) sResult += sData.GetAt(i) ;
	}

	return sResult;

}

CString GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	return sRtn;
}

BOOL IsStockAcc(CString sAcc)
{
	sAcc.Remove('-');
	if (sAcc.GetLength() >= 10)
		if (sAcc.Mid(8, 2) == "11" || sAcc.Mid(8, 1) == "6")
			return TRUE;
	return FALSE;
}

CString	makeAccn(CString accnnum, CString accnname)
{
	CString	str = _T("");
	if (accnnum.GetLength() > 8)
		str.Format("%s-%s", accnnum.Left(8), accnnum.Mid(8));
	str += (" " + accnname);
	return str;
}

CString	GetAccName(CString accn)
{
	const int nPos = accn.Find(" ");
	if (nPos >= 0)
		accn = accn.Mid(nPos + 1);

	return accn;
}

CString GetAccNum(CString accn)
{
	const int nPos = accn.Find("-");
	if (nPos >= 0)
		accn = accn.Left(nPos);

	return accn;
}
CString	GetAccGubn(CString accn)
{
	const int	nPos = accn.Find("-");
	
	if (nPos >= 0)
		accn = accn.Mid(nPos + 1, 2);

	return accn;
}


CString	Trim(CString sData)
{
	sData.TrimLeft();
	sData.TrimRight();
	return sData;
}
int	CInt(CString sData)
{
	if(sData == "") return 0;
	return atoi(sData);
}

CString	CStr(int ival)
{
	CString sResult = _T("");
	sResult.Format("%d", ival);
	return sResult;
}

bool ExistFile(CString sPath)
{

//	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(sPath)) return false;

	return true;
}


CString MakeGroupItemPacket(const CString& code, CString amount, CString price, CString name)
{
	CString sResult = code;
	sResult.TrimLeft(); sResult.TrimRight();
	if (sResult.IsEmpty())	return _T("");

	sResult += "\t";
	if (!amount.IsEmpty()) sResult += amount;
	sResult += "\t";

	if (!price.IsEmpty()) sResult += price;
	sResult += "\t";

	if (!name.IsEmpty()) sResult += name;

	return sResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//String Parsing
/////////////////////////////////////////////////////////////////////////////////////////////////////
CString ParsingString(CString &strData, CString strToken)
{
	const int iFind = strData.Find(strToken) ;
	CString sResult = _T("");
	if(iFind < 0 ) 
	{
		if(!strData.IsEmpty() ) 
		{
			sResult = strData;
			strData = "";
		}
	}
	else
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	return sResult;
}

//2
int SplitString_MAP(CString strData, CString strToken, CMapStringToString &arMapList) //
{
	if(strToken == "") return 0;
	arMapList.RemoveAll(); 
	CString strSplit, strKey = _T(""), strVal = _T("");
	int iFind = 0, index = 0;

	while(1)
	{
		iFind = strData.Find(strToken); 
		if(iFind < 0 ) 
		{
			if(index > 0)
				iFind = strData.GetLength(); 
			else break;
		}

		if(index == 0)
		{
			strKey = strData.Left(iFind);
			index++;
		}
		else 
		{
			strVal = strData.Left(iFind);
			index = 0;

			if(!strKey.IsEmpty() ) arMapList.SetAt(strKey, strVal); 
		}
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}

	return arMapList.GetCount() ;
}



CString SplitString(CString &strData, CString strToken) //
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	const int iFind = strData.Find(strToken); 
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}

	return sResult;
}

int SplitString(CString strData, CString strToken, CString *sList, int imaxcnt)
{
	if(strToken == "") return 0;
	int iFind = 0, index = 0;
	CString strlog;
	while(1)
	{
		iFind = strData.Find(strToken); 
		if(iFind < 0 ) 
		{
			if(!strData.IsEmpty() ) sList[index++] = strData;
			break;
		}
		sList[index++] = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
		if(imaxcnt < index) break;
	}

	return index;
}

int SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt)
{
	if(strToken == "") return 0;

	astrResult.RemoveAll();
	int iFind = 0;
	while(1)
	{
		iFind = strData.Find(strToken); 
		if(iFind < 0 ) 
		{
			if(!strData.IsEmpty() )astrResult.Add(strData);
			break;
		}
	
		astrResult.Add( strData.Left(iFind) ); 
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
		if(strData == "")break;
		if(imaxcnt < astrResult.GetSize() ) break;
	}

	return astrResult.GetSize(); 
}


int SplitInt(CString strData, CString strToken, int *iList)
{
	if(strToken == "") return 0;

	CStringArray astrList;
	SplitString(strData, strToken, astrList);
	for(int i=0; i<astrList.GetSize(); i++)
	{
		iList[i] = atoi(astrList[i]);
	}

	return astrList.GetSize();
}

int SplitInt(CString strData, CString strToken, CWordArray &arList)
{
	if(strToken == "") return 0;

	CStringArray astrList;
	SplitString(strData, strToken, astrList);
	for(int i=0; i<astrList.GetSize(); i++)
	{
		arList.Add( atoi(astrList[i]) );
	}

	return astrList.GetSize();
}

int SplitDouble(CString strData, CString strToken, double *iList, bool bsign)
{
	if(strToken == "") return 0;

//	const int iFind = 0, index = 0;
	CStringArray astrList;
	SplitString(strData, strToken, astrList);
	for(int i=0; i<astrList.GetSize(); i++)
	{
		iList[i] = atof(astrList[i]);
	}

	return astrList.GetSize();
//	return index;
}

int SplitDoubleD(CString strData, CString strToken, double *iList, double ldifval)
{
	if(strToken == "") return 0;

//	const int iFind = 0, index = 0;
	CStringArray astrList;
	SplitString(strData, strToken, astrList);
	for(int i=0; i<astrList.GetSize(); i++)
	{
		iList[i] = atof(astrList[i]) * ldifval;
	}

	return astrList.GetSize();
//	return index;
}

CString GetFindString(CString sData, CString sKey, CString sDefault)
{
	const int iFind = sData.Find( sKey);
	if( iFind < 0 ) return sDefault;

	sData = Trim(sData.Right(sData.GetLength() - iFind - sKey.GetLength()  ) ); 
	if(sData.Left(1) == "=") sData = sData.Mid(1, sData.GetLength() -1);    

	return sData;
}

CString GetFindOptionString(CString sData, CString sKey, CString sEndChar)
{
	int iFind = sData.Find( sKey);
	if( iFind < 0 ) return _T("");

	const int iendFind = sData.Find( sEndChar, iFind);
	if(iendFind < 0) return _T("");


	iFind = iFind + sKey.GetLength(); 
	CString sResult = sData;
	sResult = sResult.Mid(iFind, iendFind - iFind); 
	return sResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

int _httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};

	TCHAR* mstr = _tcsupr(_tcsdup(value));
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}

	[[gsl::suppress(26408)]]
	free(mstr);

	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//profile
/////////////////////////////////////////////////////////////////////////////////////////////////////
CString GetConfigFileString(CString strFile, CString strSection, CString strKey, CString strDefault)
{
	CString strResult = _T("");
	int readL = 0; 
	char readB[1024*10]; //WIN95/WIN98 MAX 32767byte
	readL = GetPrivateProfileString(strSection, strKey, strDefault, readB, sizeof(readB), strFile);
	if (readL > 0)	strResult.Format("%s", readB);

	return strResult;
}

COLORREF	GetConfigFileColor(CString strFile, CString strSection, CString strKey, CString strDefault)
{
	COLORREF	color = RGB(0,0,0);
	int readL = 0; 
	char readB[1024*10]; //WIN95/WIN98 MAX 32767byte
	readL = GetPrivateProfileString(strSection, strKey, strDefault, readB, sizeof(readB), strFile);
	if (readL > 0) color = _httoi(readB);

	return color;
}

int	GetConfigFileInt(CString strFile, CString strSection, CString strKey, int idefault)
{
	return GetPrivateProfileInt(strSection, strKey, idefault, strFile);
	/*
	CString sRead = GetConfigFileString(strFile, strSection, strKey, strDefault);
	if(sRead != "")
	{
		return atoi(sRead);
	}
	return 0;
	*/
}

void WriteConfigFileInt(CString strFile, CString strSection, CString strKey, int ival)
{
	CString sData;
	sData.Format("%d", ival); 
	WritePrivateProfileString(strSection, strKey, (LPTSTR)(LPCTSTR)sData, strFile);
}

void WriteConfigFileUInt(CString strFile, CString strSection, CString strKey, UINT nval)
{
	CString sData;
	sData.Format("%d", nval); 
	WritePrivateProfileString(strSection, strKey, (LPTSTR)(LPCTSTR)sData, strFile);
}

//GetPrivateProfileSection
int  GetProfileKeyList(CString strFile, LPCTSTR  strAppName, CStringArray& arList)
{
/*
	char sztext[1024 * 10]{};//WIN95/WIN98 MAX 32767byte
	//const int iresult = GetPrivateProfileSection(strAppName, sztext, sizeof(sztext), strFile);

	LPCSTR lpKeyName = sztext;
	arList.RemoveAll();
	
	while(*lpKeyName) 
	{
		arList.Add(lpKeyName); 
		puts(lpKeyName);
		lpKeyName += strlen(lpKeyName) + 1;
	}

	return arList.GetSize() ;
*/
	return 0;
}

///////////////////////////////////////////
CString	filltoken(CString strData, int ilen, CString strToken, 
								int itype/*0right, 1-left*/)
{
	const int iDataLen = strData.GetLength();
	
	CString strResult = strData, strTmp= _T("");

	int istart = 0;
	int iend = 0;
	if(itype == 0)
	{
		istart = iDataLen;
		iend = ilen;
		strTmp= _T("");
	}
	else
	{
		istart = 0;
		iend = ilen-iDataLen;
		strTmp = strData;
		strResult = _T("");
	}

	for(int i=istart; i<iend; i++)
		strResult += strToken;

	return strResult + strTmp;
}

CString MakeNumber(CString sData, bool bsign)
{
	if(!isNumber(sData)) return sData;
	CString sResult, sign;

	if(sData.FindOneOf("+-") == 0)
	{
		sign = sData.Left(1);
		if(!bsign && sign != "-") sign = "";

		sData = sData.Right(sData.GetLength() -1);  
	}

	bool bisnumstart = false;
	const int ilen = sData.GetLength();
	for(int i=0; i<sData.GetLength(); i++)
	{
		if(!bisnumstart && sData.GetAt(i) != '0')
		{
			bisnumstart = true;
		}

		if(bisnumstart) 
		{
			if(sData.GetAt(i) == '.' && sResult.IsEmpty()) sResult = "0";
			sResult += sData.GetAt(i);
		}
	}

	return sign + sResult;
}


CString SetComma(CString sData, int ifloatlen)
{
	if(!isNumber(sData)) return sData;

//	sData.Replace("-", "");  sData.Replace("+", "");
	CString sign = "";
	if(sData.FindOneOf("+-") == 0)
	{
		sign = sData.Left(1);
		sData = sData.Right(sData.GetLength() -1);  
	}

	sData.TrimLeft(" "); sData.TrimRight(" ");
	CString strIntVal = sData, strFloatVal="", strResult="";

	int iFind = 0;
	if( (iFind = sData.Find(".") ) > -1)
	{
		strIntVal = sData.Left(iFind);
		strFloatVal = sData.Right( sData.GetLength() - iFind ) ;
	}

	for(int i=0; i<strIntVal.GetLength(); i++)
	{
		strResult += strIntVal.GetAt(i); 
		if( (strIntVal.GetLength()-i-1)%3 == 0 && i!= strIntVal.GetLength()-1) // 
			strResult += ",";
	}

	if(strFloatVal.GetLength() > ifloatlen+1)
		strFloatVal = strFloatVal.Left(ifloatlen+1); 
	strResult += strFloatVal;

	return sign+strResult;
}


CString SetDataFormat(int iType, CString sData, CString sToken, int ifloatlen)
{
	CString sResult=sData;
	switch(iType)
	{
		case FORMAT_TEXT:
			sResult = Trim(sData);
			break;

		case FORMAT_NUMCOMMA_SIGN:
			sData = MakeNumber(sData, true);
			sResult = SetComma(sData, ifloatlen);
			break;

		case FORMAT_NUMCOMMA:
			sData = MakeNumber(sData);
			sResult = SetComma(sData, ifloatlen);
			break;

		case FORMAT_ACCOUNT:
			//101-10-000002
			sData = RemoveToken(sData, '-');
			if(sData.GetLength() < 10) return sResult; 
			sResult = sData.Left(3) + "-" + sData.Mid(3, 2) + "-" +sData.Mid(5, 6);   
			break;
		case FORMAT_YYMMDD:
			if(sData.GetLength() < 8) return sResult;
			sResult = sData.Left(4) + sToken + sData.Mid(4, 2) + sToken +sData.Mid(6, 2);   
			break;

		case FORMAT_MMDD:
			if(sData.GetLength() > 4) sData = sData.Right(4) ;
			sResult = sData.Left(2) + sToken + sData.Mid(2, 2);   

			break;

		case FORMAT_HHMMSS:
			if(sData.GetLength() < 6) return sResult;
			sResult = sData.Left(2) + sToken + sData.Mid(2, 2) + sToken +sData.Mid(4, 2);   

			break;

		case FORMAT_MMSS:
			if(sData.GetLength() > 4) sData = sData.Right(4) ;
			sResult = sData.Left(2) + sToken + sData.Mid(2, 2);   
			break;

		case FORMAT_ONLYINT:
			{
				const int ival = CInt(sData);
				sResult = SetComma(CStr(ival));
			}
			break;

	}

	return sResult;
}