#if !defined(AFX_NUMEDIT_H__F55D58D6_2969_40EC_909F_C802758C68EC__INCLUDED_)
#define AFX_NUMEDIT_H__F55D58D6_2969_40EC_909F_C802758C68EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumEdit.h : header file
//

#ifndef CLASS_EXPORT
	#define CLASS_EXPORT
#endif

class CLASS_EXPORT CNumEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumEdit)
// Construction
public:
	CNumEdit();

// Attributes
public:
	enum {VALID = 0x00, OUT_OF_RANGE = 0x01, INVALID_CHAR = 0x02, MINUS_PLUS = 0x03};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void ChangeAmount(int step);
	virtual double GetDelta();
	virtual void SetDelta(double delta);
	virtual void GetRange(double &min, double &max)const; 
	virtual void SetRange(float min, float max);
	virtual void Verbose(BOOL v);
	virtual BOOL Verbose()const; 
	virtual int IsValid()const; 
	virtual int IsValid(const CString &str)const; 

	// 20081014 �ڵ��� >>
	//virtual BOOL SetValue(float val);
	virtual BOOL SetValue(double val);
	// 20081014 �ڵ��� <<
	virtual double GetValue()const; 
	virtual ~CNumEdit();

	// 20081014 �ڵ��� �ּ������� ���� ����ؼ� �����Ѵ�. >>
	virtual void SetDeltaMinimum(CString strValue);
	virtual CString GetDeltaMinimum();

	// ��ŶŸ���� �����Ѵ�. 
	virtual void SetPacketType(CString strType);
	virtual CString GetPacketType() const;
	// 20081014 �ڵ��� <<

	// Generated message map functions
protected:
	virtual CString& ConstructFormat( CString &str, double num);
	BYTE m_NumberOfNumberAfterPoint;
	BOOL m_Verbose;
	double m_Delta, m_MinValue, m_MaxValue;

	//20081014 �ڵ��� >>
	CString m_strDeltaMinimum;				// �ּ������� ���� ����ؼ� �����Ѵ�.
	CString m_strPacketType;			// ��ŶŸ���� �Է¹޴´�.
	//20081014 �ڵ��� <<

	//{{AFX_MSG(CNumEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMEDIT_H__F55D58D6_2969_40EC_909F_C802758C68EC__INCLUDED_)
