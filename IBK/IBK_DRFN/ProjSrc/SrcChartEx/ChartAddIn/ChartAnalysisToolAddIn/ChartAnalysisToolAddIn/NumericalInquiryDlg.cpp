// NumericalInquiryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartAnalysisToolAddIn.h"
#include "NumericalInquiryDlg.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryDlg dialog


CNumericalInquiryDlg::CNumericalInquiryDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_dlgBrush.CreateSolidBrush(RGB(230, 230, 230));
	m_pFont = NULL;

	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);
}

CNumericalInquiryDlg::~CNumericalInquiryDlg()
{
	ML_SET_LANGUAGE_RES();
	if( IsWindow( GetSafeHwnd())) DestroyWindow();

	m_dlgBrush.DeleteObject();
}

// (2006/2/15 - Seung-Won, Bae) for checking Runtime Class ~ Not used yet in 2006/2/15
IMPLEMENT_DYNAMIC( CNumericalInquiryDlg, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryDlg message handlers

/*-------------------------------------------------------------------------------
 - Function    :  SetDlgBrush
 - Created at  :  2005-03-16   10:25
 - Author      :  ������
 - Description :  ��ġ��ȸâ ������ �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::SetDlgBrush(COLORREF clrDlgBrush)
{
	if(m_dlgBrush.GetSafeHandle())
		m_dlgBrush.DeleteObject();
	m_dlgBrush.CreateSolidBrush(clrDlgBrush);
}


/*-------------------------------------------------------------------------------
 - Function    :  SetResourceFont
 - Created at  :  2005-03-23   09:34
 - Author      :  ������
 - Description :  Resource Font�� �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::SetResourceFont(CFont* pFont)
{
	m_pFont = pFont;
}



/*-------------------------------------------------------------------------------
 - Function    :  GetMaxColumnString
 - Created at  :  2005-03-23   10:36
 - Author      :  ������
 - Parameters  :  pData     - list.
 -                nColIndex - �÷� �ε���.
 - Description :  Column�� ���а��߿� ���� �� ���ڿ��� �����´�. (�ֳĸ� Į���� ���̸� ���ϱ� ����)
 -------------------------------------------------------------------------------*/
LPCTSTR CNumericalInquiryDlg::GetMaxColumnString( CInquiryItem *pData, int nColIndex)
{
	CString strMaxColumnString = _T("");

	CString strFirst;
	CRowData *pRowData = NULL;
	POSITION posRow = pData->GetHeadPosition();
	while( posRow)
	{
		pRowData = pData->GetNext( posRow);

		if(nColIndex == 0)
			strFirst = pRowData->m_strName;
		else if(nColIndex == 1)
			strFirst = pRowData->m_strData;
		
		if(strMaxColumnString.GetLength() < strFirst.GetLength())
			strMaxColumnString = strFirst;
	}
	
	return (LPCTSTR)strMaxColumnString;
}


/*-------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-03-28   13:05
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� �̵���Ų��.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::MoveWindow(LPCRECT lpRect, BOOL bRepaint /* = TRUE*/, CDC* pDC /* = NULL*/)
{
	CDialog::MoveWindow(lpRect, bRepaint);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-04-06   09:59
 - Author      :  ������
 - Description :  ��ġ��ȸâ�� �̵���Ų��.
 -----------------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE */)
{
	CDialog::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2005-04-14   10:50
 - Author      :  ������
 - Description :  Window�� Simple WindowŸ��(�Ļ�Ŭ�������� �������ߴ�.)���� �ܿ�
				  �״� �ʿ����� �ʴ�...
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::OnMouseMove(CPoint point)
{
}

/*-------------------------------------------------------------------------------
 - Function    :  ReFormData
 - Created at  :  2005-04-14   10:39
 - Author      :  ������
 - Parameters  :  strData - �ٲ���� ����Ÿ.
 - Return Value:  CString - �Ҽ��� �ڸ��� ���� ����Ÿ.
 - Description :  �Ҽ����ڸ����� �����.
 -------------------------------------------------------------------------------*/
CString CNumericalInquiryDlg::ReFormData(const CString& strData, const int nMaxDecimal)
{
	if(nMaxDecimal == 0)
		return strData;
	
	CString strReFormData = strData;

	int nFindIndex = strReFormData.Find(_T("."));
	
	if(nFindIndex == -1)	// 1. �Ҽ������� ����Ÿ!
	{
		for(int i = 0; i < (nMaxDecimal + 2); i++)
			strReFormData += _T(" ");	// �Ҽ���+�Ҽ��������ڸ�����ŭ ������ �߰�.
	}
	else					// 2. �Ҽ����ִ� ����Ÿ.
	{
		int nDecimal = strReFormData.GetLength() - nFindIndex;
		// �ִ�Ҽ����ڸ����� �� ���� ��� ���� ��ŭ ���� �߰�.
		if(nDecimal < nMaxDecimal)
		{
			for(int i = 0; i < (nMaxDecimal - nDecimal); i++)
				strReFormData += _T(" ");
		}
	}

	return strReFormData;
}

CString CNumericalInquiryDlg::ReFormData(const CString& strData,double dRatio, const int nMaxDecimal)
{ 
	CString strRatio;
	strRatio.Format("(%.2f%%)",dRatio);

	CString strReFormData = strData + strRatio;
	if(nMaxDecimal == 0)
	{		
		return strReFormData;
	}
		
	int nFindIndex = strData.Find(_T("."));	
	if(nFindIndex == -1)	// 1. �Ҽ������� ����Ÿ!
	{
		for(int i = 0; i < (nMaxDecimal + 2); i++)
			strReFormData += _T(" ");	// �Ҽ���+�Ҽ��������ڸ�����ŭ ������ �߰�.
	}
	else					// 2. �Ҽ����ִ� ����Ÿ.
	{
		int nDecimal = strReFormData.GetLength() - nFindIndex;
		// �ִ�Ҽ����ڸ����� �� ���� ��� ���� ��ŭ ���� �߰�.
		if(nDecimal < nMaxDecimal)
		{
			for(int i = 0; i < (nMaxDecimal - nDecimal); i++)
				strReFormData += _T(" ");
		}
	}

	return strReFormData;
}
