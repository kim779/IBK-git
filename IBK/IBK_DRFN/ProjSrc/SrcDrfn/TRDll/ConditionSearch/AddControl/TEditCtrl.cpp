// TEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TEditCtrl.h"
#include <WINUSER.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTEditCtrl

/**************************************************************/
/*                                                            */
/*                  TEdit(Dot, Mainers Control                */
/*                                                            */
/*                                                            */
/*                      �ۼ��� : 2004. 11~                    */
/*                  �ۼ��� : �쵿�� (�θ� ���� ���)          */
/*                                                            */
/**************************************************************/
CTEditCtrl::CTEditCtrl()
{
	m_bAllClear = FALSE;
	m_bIgnoreComma = FALSE;
	m_bTimeType = FALSE;
}

CTEditCtrl::~CTEditCtrl()
{
	
}


BEGIN_MESSAGE_MAP(CTEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CTEditCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTEditCtrl message handlers


BOOL CTEditCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, int nIntegerLength, int nRealLength) 
{
	BOOL bResult = TRUE;

	bResult = CEdit::Create(WS_CHILD | WS_VISIBLE | ES_LEFT, rect, pParentWnd, nID);

	m_nIntegerLength = nIntegerLength;
	m_nRealLength = nRealLength;
	
	int nMaxLength;
	if (nRealLength > 0)
		nMaxLength = nIntegerLength + nRealLength + 1 + 1;
	else
		nMaxLength = nIntegerLength + 1;
	//LimitText(nMaxLength);
		
	return bResult;
}

void CTEditCtrl::SetInit(CString strText)
{
	m_bEscapeKey = FALSE;
	
	m_bDot = FALSE;
	m_bMainers = FALSE;
	m_strEdit.RemoveAll();
	
	m_bFirstInput = TRUE;
	
	if (strText.Find(".", 0) > 0)
		m_bDot = TRUE;
	else
		m_bDot = FALSE;
	
	if (strText.Find("-", 0) > 0)
		m_bMainers = TRUE;
	else
		m_bMainers = FALSE;
	
	for (int nIndex = 0; nIndex < strText.GetLength(); nIndex++)
		m_strEdit.Add(strText[nIndex]);
	
	SetWindowText(strText);
	SetSel(0, strText.GetLength());
	SetFocus();
}

void CTEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	PostMessage(WM_CLOSE, 0, 0);
	
	if (!m_bEscapeKey)
	{
		CString strWindow;
		GetWindowText(strWindow);
		InsertComma(strWindow);
		
		m_bAllClear = FALSE;
		
		CWnd *pWndParent = GetParent();
		pWndParent->SetWindowText(strWindow);		
	} 
	else 
	{
		
	}
}

BOOL CTEditCtrl::PreTranslateMessage(MSG* pMsg) 
{

	
	return CEdit::PreTranslateMessage(pMsg);
}

void CTEditCtrl::PostNcDestroy() 
{
	CEdit::PostNcDestroy();
}

void CTEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nCount = m_strEdit.GetSize();
	
	CString strTemp;
	if (nCount > 0)
		strTemp = m_strEdit.GetAt(nCount - 1);
	
	if ((nChar == '-') && (m_bMainers == FALSE))
	{
		if ((m_bAllClear == FALSE) && (nCount > 0))
		{
			m_bDot = FALSE;
			m_bMainers = FALSE;
			m_strEdit.RemoveAll();
			m_bAllClear = TRUE;
			
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			m_strEdit.Add((LPCTSTR)&nChar);
			m_bMainers = TRUE;
		}
		else if (nCount == 0)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			m_strEdit.Add((LPCTSTR)&nChar);
			m_bMainers = TRUE;
		}
	}
	else if (nChar == '.') 
	{
		if ((m_bDot == FALSE) && (nCount > 0))
		{
			if (strTemp != '-')
			{
				if (m_bAllClear == FALSE)
				{
					m_bDot = FALSE;
					m_bMainers = FALSE;
					m_strEdit.RemoveAll();
					m_bAllClear = TRUE;
				}
				
				if (m_nRealLength > 0)
				{
					CEdit::OnChar(nChar, nRepCnt, nFlags);
					m_strEdit.Add((LPCTSTR)&nChar);
					m_bDot = TRUE;
				}
			}
		}
	}
	else if (nChar == VK_BACK)
	{
		if (nCount > 0)
		{
			if (strTemp == '-')
				m_bMainers = FALSE;
			else if (strTemp == '.')
				m_bDot = FALSE;
			
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			m_bAllClear = TRUE;
			
			CString strTemp;
			GetWindowText(strTemp);
			if (strTemp.IsEmpty() == TRUE)
			{
				m_bMainers = FALSE;
				m_bDot = FALSE;
				
				m_strEdit.RemoveAll();
			}
			else
				m_strEdit.RemoveAt(nCount - 1, 1);
		}
		else
		{
			m_bDot = FALSE;
			m_bMainers = FALSE;
			m_strEdit.RemoveAll();
		}
	}
	else if ((nChar >= '0') && (nChar <= '9'))
	{
		if (m_bAllClear == FALSE)
		{
			m_bDot = FALSE;
			m_bMainers = FALSE;
			m_strEdit.RemoveAll();
			m_bAllClear = TRUE;
		}

		BOOL bAdd = TRUE;
		if (m_bDot == TRUE)
		{
			int nRealCnt = 1;
			BOOL bHaveDot = FALSE;
			for (int nIndex = 0; nIndex < m_strEdit.GetSize(); nIndex++)
			{
				if (bHaveDot == TRUE)
					nRealCnt++;
				else
				{
					if (m_strEdit.GetAt(nIndex) == ".")
						bHaveDot = TRUE;
				}
			}

			if (m_nRealLength < nRealCnt)
				bAdd = FALSE;
		}
		
		if (bAdd == TRUE)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			m_strEdit.Add((LPCTSTR)&nChar);
		}
	}
}

void CTEditCtrl::ValueWithComma(CString& strData)
{
	for(int nIndex = 3; nIndex < strData.GetLength(); nIndex+=4)
	{		
		if( strData[strData.GetLength() - nIndex - 1] == ',')
			continue;
		strData.Insert(strData.GetLength() - nIndex, ',');
	}
}
/*
LRESULT CTEditCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_SETTEXT)
	{
		m_szText = (LPCTSTR)lParam;
//		IDataUtilManager* pMng = (IDataUtilManager*)AfxGetPctrInterface(UUID_IDataUtilManager);
//		m_szText = pMng->GetComma(m_szText);
		m_szText = GetComma(m_szText);
//		CEdit::SetWindowText(m_szText);
		return 1;
	}
	else if(message==WM_GETTEXT)
	{
		return OnGetText(wParam, lParam);
	}

	return CEdit::WindowProc(message, wParam, lParam);
}
*/
LRESULT CTEditCtrl::OnGetText(WPARAM wParam, LPARAM lParam)
{
	int nMaxLen = (int)wParam;
	if (nMaxLen == 0)
		return 0;       // nothing copied
	LPTSTR lpszDest = (LPTSTR)lParam;

	m_szText.Remove(',');

	int nLen = m_szText.GetLength();
	memcpy(lpszDest, (LPCTSTR)m_szText, nLen);

	lpszDest[nLen] = '\0';
	return nLen+1;      // number of bytes copied
}

CString CTEditCtrl::GetComma(LPCSTR pData)
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
	if( nIndex >= 0 )
	{
		sTemp1 = sTemp.Mid(nIndex+1);
		szTmp = sTemp.Left(nIndex);
	}
	else
		szTmp = sTemp;		
	//

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

CString  CTEditCtrl::GetNumber( LPCSTR pData )
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

char* CTEditCtrl::RemoveFront(LPCSTR pData, TCHAR ch)
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

void CTEditCtrl::InsertComma(CString & str1)
{
	if(m_bIgnoreComma)
		return;
	
	CString str,strTemp1,strTemp2,strTemp3;
	str = str1;
	str.Remove(',');
	//��ȣ ó��.
	if(str.GetLength() > 0)
	{
		if(str.GetAt(0) == '-' || str.GetAt(0) == '+')
		{
			strTemp1 = str.Left(1);
			str = str.Mid(1);//��ȣ������ ���ڿ�.
		}
	}

	int pos ;
	int x,y,i;
	//�Ҽ��� �κ��� �����Ѵ�.
	if((pos = str.Find(".")) != -1)
	{
		strTemp3 = str.Mid(pos);//�Ҽ������� ���ϰ��� ������ �ִ� ����.
		str = str.Left(pos);//�Ҽ��� ���� ������ ������ �ִ�.
	}
	//���ڿ��� ���̸� ���Ѵ�.
	int nLength = str.GetLength();
	int max;//�޸��� �ִ밹��

	//���� 3�ڸ� �̻��� ���� ���.
	if((x = nLength / 3) > 0)//��->x
	{
		if((y = nLength % 3) > 0) //��Ȯ�� ������ �������� ���� ��.
			max = x;
		else if(y == 0)
			max = x-1;
		
		for(i = 0 ; i < max ; i++)//������ �޸��� �����ϴ� �κ�.
		{
			if((pos = str.Find(','))== -1)//�޸��� ���ڿ��� ���� ���.
			{
				str = str.Left(nLength-3)+","+str.Mid(nLength-3);					
			}
			else
			{
				str = str.Left(pos-3)+","+str.Mid(pos-3);					
			}
		}
		str1 = strTemp1 + str + strTemp3;		
	}
}

BOOL CTEditCtrl::SetTimeFormat()
{
	CString strTime;
	GetWindowText(strTime);

	if(strTime.GetLength() == 6)
	{
		BOOL bChk = TRUE;

		if(strTime.GetAt(0) >= '3')
			bChk = FALSE;

		if(strTime.GetAt(0) == '2' && strTime.GetAt(1) >= '4')
			bChk = FALSE;

		if(strTime.GetAt(2) >= '6')
			bChk = FALSE;

		if(strTime.GetAt(5) >= '6')
			bChk = FALSE;

		if(bChk)
		{	
			strTime.Insert(2, _T(':'));
			strTime.Insert(5, _T(':'));

			SetWindowText(strTime);
			m_strBeforeText = strTime;
		}
		else
		{
			SetWindowText(m_strBeforeText);
			CString strErrorMsg;
			strErrorMsg.Format("�ð������� �߸��Ǿ����ϴ�.");
			::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
			SetFocus();
			return FALSE;
		}
	}
	else
	{
		SetWindowText(m_strBeforeText);
		CString strErrorMsg;
		strErrorMsg.Format("�ú��� �� ���ڸ��� ���ڷθ� ǥ�����ֽʽÿ�.");	
		::MessageBox(m_hWnd, strErrorMsg, PROGRAM_TITLE, MB_OK);
		SetFocus();
		return FALSE;						
	}

	return TRUE;
}

void CTEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_bEscapeKey = FALSE;	
	BOOL bValueCheck = FALSE;
	if(nChar == VK_RETURN)
	{
		if(m_bTimeType)
		{
			SetTimeFormat();
		}
		else
		{
			CString strWindow;
			GetWindowText(strWindow);

			if(atoi(strWindow) == INT_MAX)
			{
				bValueCheck = TRUE;				
				strWindow = "999999999";
			}

			InsertComma(strWindow);
			SetWindowText(strWindow);
		}

		PostMessage(WM_CLOSE, 0, 0);
	}	

	if (nChar == VK_ESCAPE)
	{	
		CString strWindow;
		GetWindowText(strWindow);
		InsertComma(strWindow);
		SetWindowText(strWindow);

		PostMessage(WM_CLOSE, 0, 0);

		m_bEscapeKey = TRUE;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	if(bValueCheck)
		::MessageBox(m_hWnd, "�Է��� �� �ִ� �ִ밪�� 999,999,999 �Դϴ�.", PROGRAM_TITLE, MB_OK);	
}
