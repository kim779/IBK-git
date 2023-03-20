#pragma once

#include <afxtempl.h>

typedef struct _scode {
	CString strAcc;
	CString strCode;
	CString strName;
} scode;

class ACCOUNT {
public:
	ACCOUNT();
	~ACCOUNT();

	void load_acc( CString str );
	CString loadPass(CString dat, CString acct);

	CStringArray* get_account() { return &m_listAcc; }

protected:

	CStringArray				m_listAcc; // ������ ����Ʈ 
	CList<scode,scode>			m_listCode;
};



