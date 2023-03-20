
#include "stdafx.h"
#include <AtlBase.h>

#include "Util.h"
#include "MainDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDicStringEx class

IMPLEMENT_DYNAMIC(CUtil, CObject)

////////////////////////////////////////////////////////////////////////////////
// 01. function��      : {CUtil}
// 02. ��  ��  ��      : {���ؿ�}
// 03. �� �� �� ��     : {1998/07/03}
// 04. function���    : {CUtil������}
// 05. �Է�����        : {����}
// 06. �������        : {����}
// 07. return ��       : {����}
// 08. REMARKS ����    : {}
////////////////////////////////////////////////////////////////////////////////
BOOL CUtil::IsNumber(UINT nChar)
{

	if( nChar >= '0' && nChar <= '9')  return TRUE;
	else return FALSE;
}



////////////////////////////////////////////////////////////////////////////////
// 01. function��      : {}
// 02. ��  ��  ��      : {���ؿ�}
// 03. �� �� �� ��     : {}
// 04. function���    : {}
// 05. �Է�����        : {}
// 06. �������        : {}
// 07. return ��       : {���ŵ� �ܾ��}
// 08. REMARKS ����    : {}
////////////////////////////////////////////////////////////////////////////////
int CUtil::Remove(CString &strIn, TCHAR ch, int nIndex)
{
	if(nIndex==0)	return 0;
	if( nIndex == -1) return RemoveAll( strIn, ch );

	CString szTmp;
	int	nLen = strIn.GetLength();
	int	nFoundIndex=0;

	for(int i=0;   i<nLen;	i++) {
		if( strIn[i]==ch ) {
			nFoundIndex ++;
			if(nFoundIndex != nIndex) szTmp += strIn[i];
		}
		szTmp += strIn[i];
	}

	strIn.Empty();
	strIn = szTmp;
	szTmp.Empty();

	return (nFoundIndex>0)? 1:0 ;
}

int CUtil::RemoveAll(CString &strIn, TCHAR ch)
{
	CString	szTmp;
	int	nRemove=0;
	int	nLen = strIn.GetLength();

	for(int i=0;   i<nLen;  i++) {
		if(strIn[i]!=ch) {
			szTmp += strIn[i];
			nRemove++;
		}
	}

	strIn.Empty();
	strIn = szTmp;
	szTmp.Empty();

	return nRemove;
}

char* CUtil::RemoveFront(LPCSTR pData, TCHAR ch)
{
	int nLen = strlen(pData);
	int nSameCharCnt = 0;

	for(int i=0;  i<nLen ; i++) 
	{  
		if(pData[i]!=ch) return (char *)&pData[i];
		nSameCharCnt++;
	}

	//���� ���̸� ���� 
	if(ch == '0' && nSameCharCnt == nLen)
	{
		return (char*)&pData[nLen-1] ;
	}

	return (char *)pData;
}

void CUtil::GetWord(LPSTR pWord, LPSTR pLine, TCHAR chStop)
{
	int x = 0,y;

	for(x=0;((pLine[x]) && (pLine[x] != chStop));x++)
		pWord[x] = pLine[x];

	pWord[x] = '\0';
	if(pLine[x]) ++x;
	y=0;

	while(pLine[y++] = pLine[x++]);
}

int CUtil::GetString( CString& szAB, CString& szCD, TCHAR chStop)
{
	int nIndex=-1;
	nIndex = szCD.Find(chStop);
	if(nIndex==-1) return nIndex;

	szAB = szCD.Mid(0, nIndex);
	szCD = szCD.Mid( nIndex+1);
	
	return nIndex;
}

BOOL CUtil::IsOnlyNumber( LPCTSTR szData)
{
	if(strlen(szData)==0) return TRUE;

	for(int x=0; (szData[x]); x++)
		if( szData[x] > '9' || szData[x] < '0')  return FALSE;

	return TRUE;
}


CString  CUtil::GetOnlyNumber( LPCSTR szData )
{
	CString szRet=_T("");

	for(int x=0; (szData[x]); x++) {
		if( szData[x] >= '0' && szData[x] <= '9')
			szRet += szData[x];
	}

	return szRet;
}



CString  CUtil::GetNumber( LPCSTR pData )
{
	CString szRet=_T("");
	int nLen = strlen(pData);
	char	ch;

	for(int i=0;  i<nLen ; i++) {
		ch = pData[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	return szRet;
}

////////////////////////////////////////////////////////////////////////////////
// 01. function��      : {GetComma}
// 02. ��  ��  ��      : {���ؿ�}
// 03. �� �� �� ��     : {1998. 6. 29}
// 04. function���    : {���ڿ� �ĸ��� ����}
// 05. �Է�����        : {CString &D}
// 06. �������        : {void}
// 07. return ��       : {}
// 08. REMARKS ����    : {}
////////////////////////////////////////////////////////////////////////////////


CString CUtil::GetComma(LPCSTR pData)
{
	CString szRet=_T("");
	CString szTmp = pData;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	szTmp = GetNumber(szTmp);

	if(szTmp.IsEmpty())	return "" ;
	TCHAR ch=' ';
	if(szTmp[0]=='+' ||szTmp[0]=='-') 
	{
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}

	// ���� 0 ����
	CString sTemp;
	TCHAR chTemp = '0';
	sTemp = RemoveFront(szTmp, chTemp);
	
	// �Ҽ��� ���� ����
	CString sTemp1;
	int nIndex = sTemp.Find(".");
	if (nIndex == 0)
	{
		sTemp1 = sTemp.Mid(nIndex+1);
		szTmp = "0";
	}
	else if( nIndex > 0 )
	{
		sTemp1 = sTemp.Mid(nIndex+1);
		szTmp = sTemp.Left(nIndex);
	}
	else
		szTmp = sTemp;		

	// ���� 0�� ��쿡 0�� �߰�
	if( szTmp.GetLength() == 0 )
		szTmp = "0";
	
	szTmp.MakeReverse();
	int	nLen = szTmp.GetLength();	
	szRet.Empty();
 	for(int i=0;   i<nLen;   i++) 
	{
		if(i!=0 && !(i%3))	szRet += ',';
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	szRet.MakeReverse();	

	// �Ҽ��� ���� �ٽ� ����
	if( nIndex >= 0 )
	{
		szRet = szRet + "." +sTemp1;
	}

	return szRet;

}

////////////////////////////////////////////////////////////////////////////////
// 01. function��      : {GetComma}
// 02. ��  ��  ��      : {���ؿ�}
// 03. �� �� �� ��     : {1998. 6. 29}
// 04. function���    : {ù��° ���ڰ��� '+' �Ǵ� '-' ǥ�ð� �ִ��� �˻��Ѵ�.}
// 05. �Է�����        : {CString &D}
// 06. �������        : {void}
// 07. return ��       : {ǥ�ð� ������ TRUE, �ƴϸ� FALSE}
// 08. REMARKS ����    : {}
////////////////////////////////////////////////////////////////////////////////

BOOL CUtil::IsSign(LPCSTR pData)
{
	if(pData[0]=='+' || pData[0]=='-') return TRUE;
	return FALSE;
}

CString CUtil::GetMakeText(TCHAR ch, int nCnt)
{
	CString szRet;
	for(int j=0; j<nCnt; j++)	
		szRet += ch ;
	return szRet;
}

CString CUtil::SetLengthString(LPCSTR pData, int nCnt)
{
	CString strReturn;
	strReturn.Format("%-*.*s",nCnt,nCnt,pData);
	return strReturn;
	
}

CString CUtil::RemoveChar(LPCSTR pData, TCHAR ch)
{
	CString szNew;

	for( int i=0; *(pData+i); i++)
		if((pData[i])!=ch) szNew += pData[i];

	return szNew;
}

// Modify S (Add Function) 98.10.24 By JunOk Lee
BOOL CUtil::ValidNumber(LPCSTR strIn)
{                                       
	CString strTmp = strIn;
	strTmp.TrimLeft();
	strTmp.TrimRight();

	if(strTmp[0]=='+' ||strTmp[0]=='-')
		strTmp = strTmp.Mid(1);

	int nIndex = strTmp.Find(".");
	if(nIndex!=-1) {
		CString chTmp=strTmp.Mid(nIndex+1);
		nIndex = chTmp.Find(".");
		if(nIndex!=-1) return FALSE;
	}

	for(int i=0;i<strTmp.GetLength();i++) {
		if(strTmp[i]=='.') continue;
		if(!((strTmp[i] >= '0' && strTmp[i] <= '9') || strTmp[i] == '-' || strTmp[i] == '+'	|| strTmp[i] == ',')) 
			return FALSE;
	}
	
	return TRUE;
}

// Modify E (Add Function) 98.10.24 By JunOk Lee

void CUtil::TrimRight(char* pIn)
{
	int nLen = strlen(pIn);
	int nOld = nLen;
	while( ::isspace(*(pIn+nLen-1)))
	{
		nLen--;
	}
	if(nLen==nOld)	return;
	pIn[nLen] = '\0';
}

void CUtil::RemoveCharAll(CString& szData, TCHAR ch)
{
	int nLen = szData.GetLength();
	char* pData = new char[nLen+1];
	int j = 0;
	for(int i=0, j=0; i<nLen; i++){
		if(szData[i]!=ch)
			pData[j++] = szData[i];
	}
	pData[j]='\0';
	szData = pData;
	delete pData;
}

CString CUtil::AdjustMask(CString mask,CString str)
{	
	// test
	if(str == "")
	{
		return "";
	}
	// end

	int nTemp = atoi(str);
	str.Format("%d",nTemp);	
	
	CString result=""; 
	
	int j=mask.GetLength()-1;
	int i=str.GetLength()-1;
	if(j<=0)
	{	
		return str;
	}
	
	while( j >= 0)
	{
		if(i>=0)
		{
			if((mask[j] == '9') || (mask[j] == 'Z') || (mask[j] == 'z'))
			{
				result=str[i]+result;
				j--; i--;
			}
			else
			{
				result=mask[j]+result;
				j--;
			}
			
		}
		else
		{
			CString str;
			if(mask[j] == '9')
			{
				str="0";
				result=str+result;
				j--;
			}
			else if((mask[j] == 'z') || (mask[j] == 'Z'))
			{
				str="";
				result=str+result;
				j--;
			}
			else
			{
				if((j+1< mask.GetLength()-1) && (j-1 >= 0))
				{
					if((mask[j-1] != 'z') && (mask[j-1] != 'Z'))
					{
						result=mask[j]+result;
					}
				}
				j--;
			}
		}
	}
	return result;
}

CString CUtil::AdjustMask(CString mask, int nValue)
{
	CString szT;
	szT.Format("%d", nValue);
	return AdjustMask(mask, szT);		
}

CString CUtil::GetSignSimbol(int sign)
{
	switch(sign)
	{
	case 1:
		return "��";
	case 2: 
		return "��";
	case 3:
		//return "��";
		return "";
	case 4:
		return "��";
	case 5:
		return "��";
	}
	return "";
}

void CUtil::AddDashToAccount(LPCSTR _lpIn, CString& szOut)
{
	CString szIn = _lpIn;
	ASSERT(szIn.GetLength()==11);

	szIn.Insert(3, "-");
	szIn.Insert(6, "-");
	szOut = szIn;
}

// �ݾ��� ��, �� ������ ǥ��(�ݾ��� 10�������� ������ ���)
CString CUtil::GetPriceUnit(LPCSTR pData)		// JSJ_Add_040528
{

	CString szPrice, szTemp, szTemp2;
	int  nLength;

	szTemp = pData;

	// ���� 0 ����
	TCHAR chTemp = '0';
	szTemp2 = RemoveFront(szTemp, chTemp);	

	// �޸� ����
	szTemp2.Remove(',');

	nLength = szTemp2.GetLength();

	if (nLength >= 9)		// 10���̻�
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(7);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "���";

		return szPrice;
	}
	else if (nLength >= 4)	// 1���� �̻�
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(3);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "����";

		return szPrice;
	}

	if (atoi(szTemp2) == 0)
		szTemp2 = "";
	szPrice = szTemp2 + "�ʿ�";
	
	return szPrice;
}

// �ݾ��� �������� ǥ��(�ݾ��� 1�������� ������ ���)
CString CUtil::GetPriceUnitMan(LPCSTR pData)		// JSJ_Add_040528
{

	CString szPrice, szTemp, szTemp2;
	int  nLength;

	szTemp = pData;

	// ���� 0 ����
	TCHAR chTemp = '0';
	szTemp2 = RemoveFront(szTemp, chTemp);	

	// �޸� ����
	szTemp2.Remove(',');

	nLength = szTemp2.GetLength();
	
	if (nLength >= 5)	// 1���� �̻�
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(4);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2;

		return szPrice;
	}

	szPrice = "0";		// ���� ������ ��� "0" ����
		
	return szPrice;
}


// �ŷ����� ��, �� ������ ǥ��(�ŷ����� 1�� ������ ������ ���)
CString CUtil::GetVolUnit(LPCSTR pData)			// JSJ_Add_040603
{

	CString szPrice, szTemp, szTemp2;
	int  nLength;

	szTemp = pData;

	// '-'��ȣ����
	szTemp.Remove('-');

	// ���� 0 ����
	TCHAR chTemp = '0';
	szTemp2 = RemoveFront(szTemp, chTemp);	

	// �޸� ����
	szTemp2.Remove(',');

	nLength = szTemp2.GetLength();

	if (nLength >= 10)		// 10���� �̻�
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(8);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "����";

		return szPrice;
	}
	else if (nLength >= 6)	// 10���� �̻�
	{
		szTemp2.MakeReverse();
		szTemp = szTemp2.Mid(4);
		szTemp.MakeReverse();

		szTemp2 = GetComma(szTemp);
		szPrice = szTemp2 + "����";

		return szPrice;
	}

	if (atoi(szTemp2) == 0)
	{
		szPrice = "��";
	}
	else
	{
		szTemp = GetComma(szTemp2);
		szPrice = szTemp + "��";
	}
		
	return szPrice;
}


// ******************************************************************
//						���� �� ���� ȣ��
// ******************************************************************
void CUtil::GetPre_N_NextHoga(int nJongMokGubun, long lPrice, long* lUpNextValue, long* lDownNextValue) 
{
	int _lUpBid = 5;
	int _lDownBid = 5;

	if(nJongMokGubun == KOSPI)			//�ŷ���
	{	
		if(lPrice<5000)							{ _lUpBid = 5;		_lDownBid = 5;		}
		else if(lPrice == 5000)					{ _lUpBid = 10;		_lDownBid = 5;		}
		else if(lPrice>5000 && lPrice<10000)	{ _lUpBid = 10;		_lDownBid = 10;		}
		else if(lPrice == 10000)				{ _lUpBid = 50;		_lDownBid = 10;		}
		else if(lPrice>10000 && lPrice<50000)	{ _lUpBid = 50;		_lDownBid = 50;		}
		else if(lPrice == 50000)				{ _lUpBid = 100;	_lDownBid = 50;		}
		else if(lPrice>50000 && lPrice<100000)	{ _lUpBid = 100;	_lDownBid = 100;	}
		else if(lPrice == 100000)				{ _lUpBid = 500;	_lDownBid = 100;	}
		else if(lPrice>100000 && lPrice<500000)	{ _lUpBid = 500;	_lDownBid = 500;	}
		else if(lPrice == 500000)				{ _lUpBid = 1000;	_lDownBid = 500;	}
		else if(lPrice>500000)					{ _lUpBid = 1000;	_lDownBid = 1000;	}
	}
	else if(nJongMokGubun == KOSDAQ)  	//�ڽ���
	{
		if(lPrice<5000)							{ _lUpBid = 5;		_lDownBid = 5;		}
		else if(lPrice == 5000)					{ _lUpBid = 10;		_lDownBid = 5;		}
		else if(lPrice>5000 && lPrice<10000)	{ _lUpBid = 10;		_lDownBid = 10;		}
		else if(lPrice == 10000)				{ _lUpBid = 50;		_lDownBid = 10;		}
		else if(lPrice>10000 && lPrice<50000)	{ _lUpBid = 50;		_lDownBid = 50;		}
		else if(lPrice == 50000)				{ _lUpBid = 100;	_lDownBid = 50;		}
		else if(lPrice>50000)					{ _lUpBid = 100;	_lDownBid = 100;	}
	}
	else if(nJongMokGubun == ETF_KOSPI)	//ETF �ŷ���
	{
		if(lPrice<5000)							{ _lUpBid = 5;		_lDownBid = 5;		}
		else if(lPrice == 5000)					{ _lUpBid = 10;		_lDownBid = 5;		}
		else if(lPrice>5000 && lPrice<50000)	{ _lUpBid = 10;		_lDownBid = 10;		}
		else if(lPrice == 50000)				{ _lUpBid = 100;	_lDownBid = 10;		}
		else if(lPrice>50000 && lPrice<100000)	{ _lUpBid = 100;	_lDownBid = 100;	}
		else if(lPrice == 100000)				{ _lUpBid = 500;	_lDownBid = 100;	}
		else if(lPrice>100000 && lPrice<500000)	{ _lUpBid = 500;	_lDownBid = 500;	}
		else if(lPrice == 500000)				{ _lUpBid = 1000;	_lDownBid = 500;	}
		else if(lPrice>500000)					{ _lUpBid = 1000;	_lDownBid = 1000;	}
	}
	else if(nJongMokGubun == ETF_KOSDAQ)	//�ڽ���  ��������	
	{
		//ETF �ڽ��� - ETF �ڽ����� ���� ������ 5������ ����Ǿ���ϴ�. ����(2004. 08.30) ���� ����..
		_lUpBid = 5;	_lDownBid = 5;
	}

	*lUpNextValue = lPrice+_lUpBid;
	*lDownNextValue = lPrice-_lDownBid;
}


int CUtil::GetHogaTick(int nJongMokGubun, int lPrice)
{
	if(nJongMokGubun == KOSPI)			//�ŷ���
	{		
		if(lPrice<5000)					return 5;
		else if(lPrice<10000)			return 10;
		else if(lPrice<50000)			return 50;
		else if(lPrice<100000)			return 100;
		else if(lPrice<500000)			return 500;
		else							return 1000;
	}
	else if(nJongMokGubun == KOSDAQ)		//�ڽ���
	{	
		if(lPrice<5000)					return 5;
		else if(lPrice<10000)			return 10;
		else if(lPrice<50000)			return 50;
		else 							return 100;

	}
	else if(nJongMokGubun == ETF_KOSPI)	//�ŷ��� ��������
	{ 
		//ETF �ŷ���
		if(lPrice<5000)								return 5; 
		else if(lPrice>=5000 && lPrice<10000)		return 10; 
		else if(lPrice>=10000 && lPrice<50000)		return 10; 
		else if(lPrice>=50000 && lPrice<100000)		return 100; 
		else if(lPrice>=100000 && lPrice<500000)	return 500; 
		else if(lPrice>=500000)						return 1000; 
	}
	else if(nJongMokGubun == ETF_KOSDAQ)	//�ڽ���  �������� 
	{
		//ETF �ڽ��� - ETF �ڽ����� ���� ������ 5������ ����Ǿ���ϴ�. ����(2004. 08.30) ���� ����..
		return 5;
	}

	return -99999999;
}


BOOL CUtil::GetRegKey(LPCSTR szSection, LPCSTR szKey, CString& rData)
{
	CRegKey regkey;
	if(regkey.Open(HKEY_CURRENT_USER, szSection)!=ERROR_SUCCESS)
		return FALSE;
	
	rData.GetBuffer(MAX_PATH);
	DWORD	dwCount = MAX_PATH;
	BOOL	bRet = TRUE;
	if(regkey.QueryValue((LPSTR)(LPCSTR)rData, szKey, &dwCount)!=ERROR_SUCCESS)
		bRet = FALSE;
	rData.ReleaseBuffer();

	return bRet;
}