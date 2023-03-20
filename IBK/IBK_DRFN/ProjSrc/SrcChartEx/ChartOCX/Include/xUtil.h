#if !defined(AFX_XUTIL_H__8F6C92A1_58B1_11D4_B316_0048543A0D2E__INCLUDED_)
#define AFX_XUTIL_H__8F6C92A1_58B1_11D4_B316_0048543A0D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDesk.h : header file
//
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//#include "../../include/xstring.h"
#include <d3dtypes.h>		//For RGB_MAKE
#include <ComDef.h>

class CConversion;
class CUtilConvert;

class CXFLog;
class CXTrace;


#define		XCONTROL_UNKNOWN		0
#define		XCONTROL_XINPUT			100
#define		XCONTROL_XGRID			200
#define		XCONTROL_XTABLE			300

#define		XVARTYPE_NONE			0
#define		XVARTYPE_BOOL			1
#define		XVARTYPE_STRING			2
#define		XVARTYPE_RECT			3
#define		XVARTYPE_COLOR			4
#define		XVARTYPE_FONT			5

class CXString
{
	DECLARE_DYNAMIC(CXString)

public:
	// ���� Ʈ��
	static void TrimAll(CString&);

	// ���� Parsing
	// �ι�° ��Ʈ���� " \t\n\r"�� �������� ������. 
	// ������ �߽����� �պκ��� �ս�Ʈ���� �޺κ��� �ι�° ��Ʈ���� ��� ó���ȴ�.
	static void	GetWord( LPSTR, LPSTR);
	static void GetWord(CString& pWord, CString& pLine);
	static int	GetString( CString&, CString&, LPCSTR strStop=" \t\n\r" );
	static int	ReadString(CString& szRead, CString& szOrg, BOOL bSplit=TRUE);

	// ���� ó��
	// nIndex -1:All , 0: noremove , ���° �ܾ ���� �ɰ�����
	static int		Remove(CString &strIn, TCHAR ch, int nIndex=1); 
	static int		RemoveAll(CString &strIn, TCHAR ch=' ');
	static char*	RemoveFront(LPCSTR, TCHAR);

	static int		GetCharCount(LPCSTR _szData, TCHAR ch);

	// ���� ó��
	static BOOL	ValidNumber(LPCSTR strIn);
	static BOOL	IsNumber(UINT nChar);
	static BOOL	IsOnlyNumber( LPCTSTR szData);

	static CString	GetComma(LPCSTR szData);
	static CString  GetOnlyNumber( LPCSTR szData );
	static CString  GetNumber( LPCSTR szData );
};

/////////////////////////////////////////////////////////////////////////////
// 

// �ӽ� ������
#define dfMaxStringLength 4096
#include "../include/drstring.h"

class CConversion
{
public:
	static int Str2Int(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		if(str.GetLength()>0)
			return atoi(str);
		return 0;
	}

	static long Str2Long(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		if(str.GetLength()>0)
			return atol(str);
		return 0L;//
	}

	static CRect Str2Rect(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		int left,top,right,bottom;
		sscanf(str,"%d,%d,%d,%d", &left, &top, &right, &bottom);
		CRect tmpRect = CRect(left,top,right,bottom);
		return tmpRect;
	}

	static COLORREF Str2RGBColor(CString& str)
	{
		int red,green, blue;
		sscanf(str, "%d,%d,%d", &red, &green, &blue);
	
		COLORREF tmpColor = RGB(red,green,blue);
		return tmpColor;
	}

	// Hi~, char* version.. added by Choi Jin Won(2003.02.20)
	static COLORREF Str2RGBColor(char* pchData)
	{
		int red,green, blue;
		sscanf(pchData, "%d,%d,%d", &red, &green, &blue);
	
		COLORREF tmpColor = RGB(red,green,blue);
		return tmpColor;
	}

	static CString Color2String(COLORREF color)
	{
		CString szReturn;

		szReturn.Format("%d,%d,%d",GetRValue(color),GetGValue(color), GetBValue(color));

		return szReturn;
	}

	static BOOL Str2Boolean(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		if(str == _T("TRUE") || str == _T("1"))
			return TRUE;
		else
			return FALSE;
	}

	static CSize Str2Size(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);
		
		CSize size(0,0);

		int cx,cy;
		int nRet = sscanf(str, "%d,%d", &cx, &cy);

		if(nRet == 2)
			size = CSize(cx,cy);
		return size;
	}

	static void String2Olechar(LPCSTR _szFrom, OLECHAR& szTo, int nLen)
	{
		MultiByteToWideChar(CP_ACP, 0, (char*)_szFrom, -1, &szTo, nLen);
	}

	static LOGFONT Str2LogFont(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		char szFontName[LF_FACESIZE] = {0,};
		double size = 10;
		int italic,underline,strikeout,weight;
		italic = underline = strikeout = weight = 0;

		//FontName/Size/Italic/UnderLine/StrikeOut/Weight
		int nFieldCount = sscanf(str,"%[^,],%lf,%d,%d,%d,%d", szFontName,&size,&italic,&underline,&strikeout,&weight);

		CClientDC dc(NULL);
		LOGFONT lf;
		lstrcpy(lf.lfFaceName, (LPCTSTR)szFontName);
		lf.lfHeight = MulDiv((int)size, dc.GetDeviceCaps(LOGPIXELSY), 72);
		lf.lfWidth = 0;
		lf.lfWeight = weight;
		
		lf.lfItalic = italic;
		lf.lfUnderline = underline;
		lf.lfStrikeOut = strikeout;

		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		lf.lfPitchAndFamily = DEFAULT_PITCH; 
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfCharSet = HANGEUL_CHARSET;                                                
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;

		return lf;

	}

	// "A=B" StringList�� CMapStringToString�� �����ϴ� �Լ�
	// CDrString Ŭ���� ����Ͽ� �ӵ� ����...
	// ������ : ������(2003.02.20)
	static void InitMapStringToString(CStringList* pList, CMapStringToString& map)
	{
		if(pList== NULL) return ;

		CString szOneItem = _T("");
		CString szKey = _T("");
		CString szValue = _T("");		
		for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
		{
			szOneItem = pList->GetNext(pos);
			int index = index = szOneItem.Find('=');
			if(index < 0) continue;

			CDrString::Left(szOneItem, szKey, index);
			CDrString::Mid(szOneItem, szValue, index+1);

			map[szKey] = szValue;
		}
	}

	static void InitArrayStringToString(CStringList* pList, CArray <CString, CString>& array)
	{
		if(pList== NULL) return ;

		for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
		{
			CString szOneItem = _T("");
			CString szKey = _T("");
			CString szValue = _T("");
			int indexArray = 0;

			szOneItem = pList->GetNext(pos);
			int index = szOneItem.Find('=');
			if(index < 0) continue;
			szKey = szOneItem.Left(index);
			szValue = szOneItem.Mid(index+1);

			indexArray = atoi(szKey);
			array[indexArray] = szValue;
		}
	}
	
	// �� �̻���� ���ؼ� ���� �Լ�....
	// ���� ����ϰ� ���� �ʽ��ϴ�.
	// 2003.02.21 ������
	static BOOL findStringinStringList(CStringList* pList, CString strQuery, int nCurCnt, CString& strResult)
	{
		CString strTemp;
		CString strTemp2;		
		
		POSITION pos = pList->FindIndex(nCurCnt);
		strTemp = pList->GetAt(pos);

		int index = strTemp.Find('=');
		CDrString::Left(strTemp, strTemp2, index);

		if( strQuery != strTemp2 )	return FALSE;
		
		CDrString::Mid(strTemp, strResult, index+1);

		return TRUE;
	}
};

class CIniFileMgr
{
public:

public:
	static COLORREF GetColor(LPCTSTR lpAppName,       // section name
							 LPCTSTR lpKeyName,        // key name
							 LPCTSTR lpDefault,        // default string
							 LPCTSTR lpFileName)       // initialization file name
	{
		COLORREF color;
		char tmp[128];
		::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,tmp,32,lpFileName);
		color=CConversion::Str2RGBColor(tmp);
		return color;
	}

	static CString GetString(LPCTSTR lpAppName,  // section name
							 LPCTSTR lpKeyName,  // key name
						     LPCTSTR lpDefault,  // default string
							 DWORD	 nSize,		 //size of destination buffer
						     LPCTSTR lpFileName) // initialization file name
	{
		CString szRet;
		char tmp[128];
		::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,tmp,nSize,lpFileName);

		szRet.Format("%s",tmp);
		return szRet;

	}

	static int GetInt(  LPCTSTR lpAppName,  // section name
						LPCTSTR lpKeyName,  // key name
						INT nDefault,       // return value if key name not found
						LPCTSTR lpFileName)  // initialization file name
	{
		int value = ::GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,lpFileName);
		return value;
	}
		

};

///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////
//Start EX

#define NEO_CONVERT_BEGIN_EX(STR_LIST)	\
	CArray <CString, CString>  strArray;\
	strArray.SetSize(100);\
	CConversion::InitArrayStringToString(STR_LIST, strArray);\
	int	nArraySize = strArray.GetSize();\
	CString szTempEx = _T("");
	
	
#define NEO_CONVERT_END_EX()\
	strArray.RemoveAll();

#define NEO_CONVERT_INT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Int(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_STRING_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = szTempEx;\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LONG_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Long(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

		
#define NEO_CONVERT_COLOR_EX(INDEX, VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2RGBColor(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_BOOL_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Boolean(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LOGFONT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2LogFont(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;


#define NEO_CONVERT_RECT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Rect(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;



//////////////////////////////////////////////////////////////////////
#define NEO_CONVERT_BEGIN(STR_LIST)	\
	CMapStringToString map;\
	CConversion::InitMapStringToString(STR_LIST, map);\
	BOOL bRet = FALSE;\
	CString szTemp = _T("");
	
#define NEO_CONVERT_END()\
	map.RemoveAll();

#define NEO_CONVERT_INT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Int(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_STRING(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE =  szTemp;\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LONG(STR_LIST, NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2Long(szTemp);\
	else \
		VARIABLE = DEFVAL;
	
		
#define NEO_CONVERT_COLOR(STR_LIST,NAME, VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2RGBColor(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_BOOL(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Boolean(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LOGFONT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2LogFont(szTemp);\
	else\
		VARIABLE  = DEFVAL;

#define NEO_CONVERT_RECT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Rect(szTemp);\
	else\
		VARIABLE= DEFVAL;


#define NEO_CONVERT_LOGFONT2(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = szTemp;\
	else\
		VARIABLE  = DEFVAL;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CMap�� ������� �ʴ� ���.. ������ 2003. 02. 19
// ����� ���ǰ� ���� ���� ��ũ���Դϴ�...
#define NEO_CONVERT2_BEGIN(STR_LIST, STR_LIST_CNT)	\
	CString szTemp = _T("");			\
	int x_nListCtn = STR_LIST_CNT;		\
	BOOL bRet = FALSE;					\
	int x_nCurCnt = 0;
	
#define NEO_CONVERT2_END() ;

#define NEO_CONVERT2_INT(STR_LIST,NAME,VARIABLE,DEFVAL)	\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Int(szTemp);\
	else \
		VARIABLE = DEFVAL;\
	x_nCurCnt++;

#define NEO_CONVERT2_STRING(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE =  szTemp;\
	else \
		VARIABLE = DEFVAL;	\
	x_nCurCnt++;

#define NEO_CONVERT2_LONG(STR_LIST, NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2Long(szTemp);\
	else \
		VARIABLE = DEFVAL;	\
	x_nCurCnt++;
	
		
#define NEO_CONVERT2_COLOR(STR_LIST,NAME, VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2RGBColor(szTemp);\
	else \
		VARIABLE = DEFVAL;	\
	x_nCurCnt++;

#define NEO_CONVERT2_BOOL(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Boolean(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT2_LOGFONT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2LogFont(szTemp);\
	else\
		VARIABLE  = DEFVAL;	\
	x_nCurCnt++;

#define NEO_CONVERT2_RECT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Rect(szTemp);\
	else\
		VARIABLE= DEFVAL;	\
	x_nCurCnt++;


#define NEO_CONVERT2_LOGFONT2(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");	\
	bRet = CConversion::findStringinStringList(STR_LIST,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = szTemp;\
	else\
		VARIABLE  = DEFVAL;	\
	x_nCurCnt++;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// 2003. 02 .14 (������)
// CAdditionToStringList 
// "A=B" ������ ��Ʈ������ �������� �����͸� �Է¹��� StringList�� �߰��Ѵ�.
//////////////////////////////////////////////////////////////////////////////
#define cnstMaxCharLength 1024*10 // 100K
class CAdditionToStringList	
{
// constructor & destructor	
public:
	CAdditionToStringList() {  }
	virtual ~CAdditionToStringList() {}

// operation for temporary data setting
public:
	void createTempdata()
	{
		;
	}
	void setTempdataToNULL()
	{
		memset(m_lpstrTemp, NULL , sizeof(m_lpstrTemp));
	}
	void destroyTempdata()
	{
		;
	}
	
// main operation 
public:
	// for "A = integer type"
	void addInteger(CStringList* strlist, CString strLhs, int nRhs, int nDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( nRhs == nDef ) return;
		
		sprintf(m_lpstrTemp, "%s=%d", (LPCSTR)strLhs, nRhs);
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();
	}
	// for "A = string type"
	void addString(CStringList* strlist, CString strLhs, CString strRhs, CString strDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( strRhs == strDef ) return;

		int nRhsLength = strRhs.GetLength();

		if( nRhsLength < cnstMaxCharLength )
		{
			sprintf(m_lpstrTemp, "%s=%s", (LPCSTR)strLhs, (LPCSTR)strRhs);
			strlist->AddTail(m_lpstrTemp);
		}
		else
		{
			CString strTemp;
			strTemp.Format("%s=%s", (LPCSTR)strLhs, (LPCSTR)strRhs);
			strlist->AddTail(strTemp);
		}

		setTempdataToNULL();
	}
	// for "A = rect type"
	void addRect(CStringList* strlist, CString strLhs, CRect rectRhs, CRect rectDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( rectRhs == rectDef ) return;
		
		sprintf(m_lpstrTemp, "%s=%d,%d,%d,%d", strLhs, rectRhs.left, rectRhs.top, rectRhs.right, rectRhs.bottom);
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();
	}
	// for "A = color type"
	void addColor(CStringList* strlist, CString strLhs, COLORREF colorRhs, COLORREF colorDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( colorRhs == colorDef ) return;
		
		sprintf(m_lpstrTemp, "%s=%d,%d,%d", (LPCSTR)strLhs, GetRValue(colorRhs), GetGValue(colorRhs), GetBValue(colorRhs));
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();		
	}
	// font �κ��� �� �̻��ϴ�... Ȯ���� ���ذ� �ʿ��ϴ�.
	void addFont(CStringList* strlist, CString strLhs, LOGFONT fontRhs, LOGFONT fontDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( memcmp(&fontRhs, &fontDef, sizeof(LOGFONT)) != 0 ) return;

		CClientDC dc(NULL);
		LONG lSize;
		lSize = MulDiv(72, fontRhs.lfHeight, dc.GetDeviceCaps(LOGPIXELSY));

		sprintf(m_lpstrTemp, "%s,%lf,%d,%d,%d,%d", fontRhs.lfFaceName, lSize, fontRhs.lfItalic,
											fontRhs.lfUnderline, fontRhs.lfStrikeOut, fontRhs.lfWeight);
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();
	}
	
// attribute
private:
	char m_lpstrTemp[cnstMaxCharLength];
};
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// NEO_ADDTOSTRLIST series											//
// (2003.2.14) ������												//
// �� �Լ����� START�� END�� �ѹ��� �������...						//
// ������Ʈ�� ������Ƽ�� StringList�� �ִ´�.						//
//////////////////////////////////////////////////////////////////////
#define NEO_ADDTOSTRLIST_START	\
	CAdditionToStringList classA;	\
	classA.createTempdata();

#define NEO_ADDTOSTRLIST_END		\
	classA.destroyTempdata();

#define NEO_ADDTOSTRLIST_BOOL(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addInteger(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_INT(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addInteger(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_STRING(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addString(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_LONG(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addInteger(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_COLOR(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addColor(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_LOGFONT(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addFont(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_RECT(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addRect(STR_LIST,NAME,VARIABLE, DEFVAL);

#define NEO_ADDTOSTRLIST_LOGFONT2(STR_LIST,NAME,VARIABLE, DEFVAL)	\
	classA.addString(STR_LIST,NAME,VARIABLE, DEFVAL);
//////////////////////////////////////////////////////////////////////



#define XCONVERT_BEGIN()	\
		/* TRACE("XCONVERT_BEGIN\n");*/	\
		CString tmpString;

#define XCONVERT_END()	\
		/* TRACE("XCONVERT_END\n");*/;

#define XCONVERT_INT(theMap, XXXX, YYYY, DEFXXX) \
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	\
			YYYY = CConversion::Str2Int(tmpString);	\
		else YYYY = DEFXXX;


#define XCONVERT_STRING(theMap, XXXX,YYYY, DEFXXX) \
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString)) YYYY = tmpString;	\
		else YYYY = DEFXXX;

#define XCONVERT_LONG(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString)) YYYY = CConversion::Str2Long(tmpString);	\
		else YYYY = DEFXXX;

#define XCONVERT_RECT(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	YYYY = CConversion::Str2Rect(tmpString);	\
		else YYYY = DEFXXX;

#define XCONVERT_COLOR(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	YYYY = CConversion::Str2RGBColor(tmpString);	\
		else YYYY = DEFXXX;

#define XCONVERT_BOOL(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	YYYY = CConversion::Str2Boolean(tmpString);	\
		else YYYY = DEFXXX;

class CUtilConvert
{
public:

public:
	static BOOL ParseComponent(CString& szValue);
	static BOOL ParseWsStyle(LPCSTR _szValue, DWORD& _dwStyle);
	static BOOL ParseWsExStyle(LPCSTR _szValue, DWORD& _dwExStyle);

	static int	 GetCharCount(LPCSTR _szData, char ch);
};

class CWLCInfo
{
public:
	CString m_szWindowName;
	CRect	m_rect;
	UINT	m_nID;
	DWORD	m_dwStyle;
	DWORD	m_dwExStyle;
};

class CXTrace
{
public:
	CXTrace()
	{
		m_nType = xt_DEBUG;
	}

	CXTrace(int nType)
	{
		m_nType = nType;
	}


	void Trace(LPCSTR szMsg)
	{
		if(m_nType & xt_DEBUG)
			TRACE("%s\n", szMsg);
		if(m_nType & xt_FILE) {
			int nSwitch = AfxGetApp()->GetProfileInt("DEBUG", "Switch", 0);
			if(nSwitch=222) {
				CString szFile = AfxGetApp()->GetProfileString("DEBUG", "XDebug", "");
				if(szFile.GetLength()>0) {
					FILE *fp = fopen(szFile, "at");
					if(fp) {
						fprintf(fp, "%s\n", szMsg);
						fclose(fp);
					}
				}
			}
		}
	}

	int		m_nType;
	enum {
		xt_DEBUG=1,
		xt_FILE=2,
	} xtrace_Type;
};

class CXFLog
{
public:
	static void Trace(LPCTSTR lpszFormat, ...)
	{
		if(!lpszFormat)
			return;

		va_list args;
		va_start(args, lpszFormat);

		int nBuf;
		TCHAR szBuffer[512];

		nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);

		// was there an error? was the expanded string too long?
		ASSERT(nBuf >= 0);

		va_end(args);

		CString szMsg = szBuffer;

		CXTrace xTrace(CXTrace::xt_FILE);
		xTrace.Trace(szMsg);
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XUTIL_H__8F6C92A1_58B1_11D4_B316_0048543A0D2E__INCLUDED_)
