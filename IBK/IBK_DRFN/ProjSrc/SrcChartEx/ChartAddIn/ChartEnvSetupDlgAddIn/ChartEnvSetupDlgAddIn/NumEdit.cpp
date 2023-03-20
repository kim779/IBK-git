// NumEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Float.h"
#include "NumEdit.h"

#include "../Include_Chart/Conversion.h"	// 20081014 �ڵ���

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumEdit

IMPLEMENT_DYNAMIC(CNumEdit, CEdit)

CNumEdit::CNumEdit()
{
	m_NumberOfNumberAfterPoint = 0;
	m_Verbose = FALSE;
	m_MinValue = -FLT_MAX;
	m_MaxValue = FLT_MAX;
	m_Delta = FLT_ROUNDS;

	//20081014 �ڵ��� >>
	m_strDeltaMinimum = "";

	m_strPacketType = "";
	//20081014 �ڵ��� <<
}

CNumEdit::~CNumEdit()
{
}


BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	//{{AFX_MSG_MAP(CNumEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumEdit message handlers

double CNumEdit::GetValue() const
{
	// 20081014 �ڵ��� ������ȯ ���ο� ���� ���� �ٸ��� �����´�. >>
	double f = 0.0;

//	if (IsValid() == VALID)
//	{
//		CString str;
//		GetWindowText(str);
//
//		sscanf(str, "%f", &f);
//	}

	if(CDataConversion::IsBaseFormat(GetPacketType()))
	{
		f = m_MinValue;

		CString str;
		CString strOutput;

		GetWindowText(str);
		
		BOOL bResult = CDataConversion::GetDataToDec(str, GetPacketType(), strOutput);	
		if(bResult)
			f = atof(strOutput);
	}
	else
	{
		f = m_MinValue;

		if (IsValid() == VALID)
		{
			CString str;
			GetWindowText(str);
			sscanf(str, "%lf", &f);
		}
	}
	// 20081014 �ڵ��� <<

	return f;
}

// 20081014 �ڵ��� >>
//BOOL CNumEdit::SetValue(float val)
BOOL CNumEdit::SetValue(double val)
// 20081014 �ڵ��� <<
{
	// 20081014 �ڵ��� ������ȯ ���ο� ���� ���� �ٸ��� �����´�. >>
//	if (val > m_MaxValue || val < m_MinValue) return FALSE;
//	CString str, s;
//	str.Format(ConstructFormat(s, val), val);
//	SetWindowText(str);
	CString strValue = "";

	if(CDataConversion::IsBaseFormat(GetPacketType()))
	{
		strValue = CDataConversion::GetStringThousandFormat(val, GetPacketType());

		SetWindowText(strValue);
	}
	else
	{
//>> vntsorl(20090828)
		CString strType = GetPacketType();
		if(strType.Find('%') == -1)
		{
			if (val > m_MaxValue || val < m_MinValue) return FALSE;
			CString str, s;
			str.Format(ConstructFormat(s, val), val);
			SetWindowText(str);
		}
		else
		{
			strType.Remove('%');

			int nPoint = 0;
			int nPos = strType.Find('.');
			if(nPos >= 0)
			{
				CString strPoint = strType.Mid(nPos+1);
				nPoint = strPoint.GetLength();
			}

			strValue.Format("%.*f", nPoint, val);

			SetWindowText(strValue);
//<< vntsorl(20090828)
		}
	}
	// 20081014 �ڵ��� 

	return TRUE;
}

int CNumEdit::IsValid() const
{
	CString str;
	GetWindowText(str);
	int res = VALID;
	if((str.GetLength() == 1) && ((str[0] == '+') || (str[0] == '-')))
		res = MINUS_PLUS;
	else{
		char lp[10];
		float f = 0.0;
		if (sscanf(str, "%f%s", &f, lp) != 1)
			res = INVALID_CHAR;
		else{
			if (f > m_MaxValue || f < m_MinValue) 
				res = OUT_OF_RANGE;
		}
	}

	if (m_Verbose && (res != VALID) && (res != MINUS_PLUS)) {
		str.Empty();
		if (res & OUT_OF_RANGE) str += _T("Given value is out of range.\n");
		if (res & INVALID_CHAR) str += _T("Characters must be a number.\n");
		AfxMessageBox(str, MB_OK | MB_ICONSTOP);
	}
	return res;
}

int CNumEdit::IsValid(const CString &str) const
{
	int res = VALID;
	float f;
	char lp[10];
	if ((str.GetLength() == 1) && ((str[0] == '+') || (str[0] == '-'))) res = MINUS_PLUS;
	else if (sscanf(str, "%f%s", &f, lp) != 1) res = INVALID_CHAR;
	else if (f > m_MaxValue || f < m_MinValue) res = OUT_OF_RANGE;
	if (m_Verbose && (res != VALID) && (res != MINUS_PLUS))
	{
		CString msg;
		msg.Empty();
		if (res & OUT_OF_RANGE) msg += _T("Given value is out of range.\n");
		if (res & INVALID_CHAR) msg += _T("Characters must be a number.\n");
		AfxMessageBox(str, MB_OK | MB_ICONSTOP);
	}
	return res;
}

void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// 20081014 �ڵ��� ������ȯ������ ���ο� ���� �Է¹޴´�. ������ȯ�̸� TRUE, �ƴϸ� FALSE >>
	if(CDataConversion::IsBaseFormat(GetPacketType()))
	{
		// ������ȯ�� ���...
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
	{
		// ������ȯ�� �ƴ� ���...
		if (nChar == ' ') return;
		double oldValue;
		oldValue = GetValue();
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		int val = IsValid();
		CString s;
		switch (val)
		{
			case VALID:
				ConstructFormat( s, GetValue());
				break;
			case MINUS_PLUS: break; 
			default:
				SetValue(oldValue);
				SetSel(0, -1);
				MSG msg;
				while (::PeekMessage(&msg, m_hWnd, WM_CHAR, WM_CHAR, PM_REMOVE));
				break;
		}
	}
	// 20081014 �ڵ��� <<
}

// 20081014 �ڵ��� >>
void CNumEdit::SetDeltaMinimum(CString strValue)
{
	m_strDeltaMinimum = strValue;
}

CString CNumEdit::GetDeltaMinimum()
{
	return m_strDeltaMinimum;
}

void CNumEdit::SetPacketType(CString strType)
{
	m_strPacketType = strType;
}

CString CNumEdit::GetPacketType() const
{
	return m_strPacketType;
}
// 20081014 �ڵ��� <<

BOOL CNumEdit::Verbose() const
{
	return m_Verbose;
}

void CNumEdit::Verbose(BOOL v)
{
	m_Verbose = v;
}

void CNumEdit::SetRange(float min, float max)
{
	m_MinValue = min;
	m_MaxValue = max;
}

void CNumEdit::GetRange(double & min, double & max) const
{
	min = m_MinValue;
	max = m_MaxValue;
}

void CNumEdit::SetDelta(double delta)
{
	m_Delta = delta;
}

double CNumEdit::GetDelta()
{
	return m_Delta;
}

void CNumEdit::ChangeAmount(int step)
{
	// 20081014 �ڵ��� ������ȯ������ ���ο� ���� ���� ���� �ٸ��� �Ѵ�. >>
	double f=0.0;
	CString strOutput;

	CString temp;
	GetWindowText(temp);
	temp.TrimLeft(); temp.TrimRight();
	
	// ��ĭ���� ���ɴ������� ó��...
	if(temp == "")
		f = 0.0;
	else
	{		
		if(CDataConversion::IsBaseFormat(GetPacketType()) && (GetDeltaMinimum() != ""))
		{
			BOOL bResult = CDataConversion::GetDataToDec(GetDeltaMinimum(), GetPacketType(), strOutput);

			if(bResult)
			{
				f = GetValue() + step * atof(strOutput);		// ������ȯ�� ���...
				//double a = GetValue();
				//double b = step * atof(strOutput);
				//f = a + b;
			}
		}
		else
			f = GetValue() + step * m_Delta;					// ������ȯ�� �ƴ� ���...
	}
	
	if (f > m_MaxValue) f = m_MaxValue;
	else if (f < m_MinValue) f = m_MinValue;
	SetValue(f);
	// 20081014 �ڵ��� <<
}

CString& CNumEdit::ConstructFormat(CString &str, double num)
{
	str.Format("%G", num);
	int n = str.Find('.');
	if (n >= 0)
	{
		n = str.GetLength() - n - 1;
		if (n > m_NumberOfNumberAfterPoint) m_NumberOfNumberAfterPoint = n;
	}
	str.Format("%%.%df", m_NumberOfNumberAfterPoint);
	return str;
}
