#pragma once

#include "AxisExt.h"

#define NOTIFY_COLS		(13)
/*
 INDEX Table
	 0 : �����ڵ�
	 1 : �����
	 2 : �������� ( +�ż� , -�ŵ� )
	 3 : �ܰ����
	 4 : û�����
	 5 : ��մܰ�
	 6 : ���簡
	 7 : Unknown
	 8 : �򰡱ݾ�
     9 : �򰡼���
	10 : ���ͷ�
	11 : ���Աݾ�
	12 : Unknown
*/

class CNotifyCtrl : public CWnd, public CAxisExt
{
public:
	CNotifyCtrl(CWnd *pParent, LPCSTR type, int key);
	void Send(_bstr_t acno, _bstr_t pswd, _bstr_t code);
	int  GetKey() { return m_key; }
	CWnd *m_pCtrl;

protected:
	void CreateControl();
	void ErrorBox( LPCSTR msg );
	void ChangeFont( LPCSTR font, int point );
	CString GetCtrlProperty( _bstr_t prop_name );
	void SetCtrlProperty( _bstr_t prop_name, _variant_t &var );
	void CheckData();
protected:
	BOOL m_bLogging;
	CString m_logpath;
	
	CString m_acno;
	LPCSTR m_type;
	int m_key;
	HINSTANCE m_hInst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotifyCtrl)
	//}}AFX_VIRTUAL

public:
	virtual ~CNotifyCtrl();

protected:
	//{{AFX_MSG(CNotifyCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser( WPARAM wParam, LPARAM lParam );
};

