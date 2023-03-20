// 2000. 11.24 Junok Lee

#ifndef __LOCALUTIL__HEADER__
#define __LOCALUTIL__HEADER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocalUtil.h : header file
//
	
//#include "../../ToolKit/CodeUtil/codedef.h"
#define		CODELEN_F			8			// �����ڵ� ����
#define		NAMELEN_F			30			// ��������� ����

/////////////////////////////////////////////////////////////////////////////
// CCodeUtil window

class CCodeUtil
{
// Construction
public:
	CCodeUtil();

// Attributes
public:

// Operations
public:
	static int  Read_File(LPCSTR szFile, CStringList& list);
	static void GetFileName(LPCSTR szKey, CString& szName, LPCSTR _szINI);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeUtil)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeUtil();

};

class CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr) { return FALSE; }
};

//#define	ONESIZE_JMASTER	(1+1+18+6+7+3+1+1+3)
#define	ONESIZE_JMASTER	(1+1+18+6+7+3+1+1+3)
class CJMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szUnder;		// 1  ����/��������
	CString m_szWarn;		// 1  �������� ����
	CString m_szName;		// 18 �����
	CString m_szCode;		// 6  �����ڵ�
	CString m_szSymbol;		// 7  �ɹ�
	CString m_szGubun;		// 1  ��������
	CString m_szUpCode;		// 3  �����ڵ�
	CString m_szS;			// 1  �α���
	CString m_szC;			// 1  ���߼�
	CString m_szG;			// 3  �׷��ڵ�
	CString m_manuf;		// 1  "1" ������
	CString m_szUpCode2;	// 3  ������ ���� �ʵ� �߰�
};

// �ڼ��� 2002.09.15
// JMaster �� �׷�� ��� �߰�.(������ JMaster �� ����)
// �ڼ��� 2002.09.18 => SK KMaster.dat ������ �غ� �ȵ�
#define	ONESIZE_KMASTER	(1+1+18+7+7+1+3+3+3+1+1+3)
class CKMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szUnder;		// 1  ����/��������
	CString m_szWarn;		// 1  �������� ����
	CString m_szName;		// 18 �����
	CString m_szCode;		// 7  �����ڵ�
	CString m_szGubun;		// 1  ����
	CString	m_szUpCode;		// 3  �����ڵ�
	
//	CString m_szG;			// 3  �׷��ڵ�
//	CString m_szVen;		// 1  ��ó����
	CString m_szUpCodem;	// 3  �����ڵ�(�ߺз� 020 ~ 049)
	CString m_szUpCodel;	// 3  �����ڵ�(��з� 002 ~ 019)
	CString	m_szCapsize;	// 1  �ð��Ѿ� '1':�� '2':�� '3':�� */
	                                /* '1':KOSDAQ100 '2':KOSDAQMID300 '3':KOSDAQSMALL */
//005 2004.01.29
	CString m_szVen;		// 1  ��ó����
	CString m_stargb;		// 1  ��Ÿ����'1'
	CString m_szG;			// 3  �׷��ڵ�	
};

#define	ONESIZE_UMASTER	(18+3)
class CUMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 ������
	CString m_szGubun;		// 1 ����
	CString m_szCode;		// 3  �����ڵ�
};

#define	ONESIZE_KUMASTER	(18+3)
class CKUMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 ������
	CString m_szGubun;		// 1 ����
	CString m_szCode;		// 3  �����ڵ�
};

#define ONESIZE_UPMASTER (18+3)
class CUPMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 ������
	CString m_szGubun;		// 1 ����
	CString m_szCode;		// 3  �����ڵ�
};

#define ONESIZE_UDMASTER (18+3)
class CUDMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 18 ������
	CString m_szGubun;		// 1  ��������
	CString m_szCode;		// 3  �����ڵ�
};

// ��3����
#define	ONESIZE_TMASTER	(40+6)
class CTMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 40 �����
	CString m_szCode;		//   �����ڵ�
};

#define	ONESIZE_GMASTER	(40+3)
class CGMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 40 �׷��
	CString	m_szGubun;		// 1  ����
	CString m_szCode;		// 3  �׷��ڵ�
};

// ����
// #define	ONESIZE_FMASTER	(20+5)
#define	ONESIZE_FMASTER	(NAMELEN_F+CODELEN_F)
class CFMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 30 �׷��
	CString m_szCode;		// 8  �׷��ڵ�
};

// �ɼ�
#define	ONESIZE_PMASTER	(14+8)
class CPMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 14 �׷��
	CString m_szCode;		// 8  �׷��ڵ�
};

// �׸�
#define	ONESIZE_THMASTER	(40+3)
class CTHMaster : public CMasterBase
{
public:
 	virtual BOOL ReadData(LPCSTR _szStr);
 
 	CString m_szName;		// 40 �׷��
//	CString	m_szGubun;		// 1 ����
 	CString m_szCode;		// 2  �׸��ڵ�
};

// * ������
#define		ONESIZE_DMaster			(7)
class CDMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szDate;		// 7 ������
};

// �ڼ��� 2002.06.27
// ����, ���ܸ� �����ϱ� ���ؼ�
// * �ŷ����ڵ�
#define		ONESIZE_SMaster		(20 + 3 + 1)
class CSMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 20 ���ǻ��
	CString	m_szCode;		//  3 �ڵ�
	CString m_szGubun;		//  1 ����(0), �ܱ�(1)
};

//  * ä��
#define		ONESIZE_CMaster			(45+9)
class CCMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	
	CString m_szName,		// 45 ä�Ǹ�
			m_szCode;		//  9 �ڵ�
};

// * �ڽ��ڼ���
#define		ONESIZE_KFMASTER	(30 + 11)
class CKFMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;
	CString m_szCode;
};

// * �ڽ��ڼ����ռ�
#define		ONESIZE_KFTSMASTER	(15)
class CKFTSMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;
	CString m_szName;
};

/* ��������,�����μ���  Master Layout */
class CBMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;		// (20)�����
	CString m_szName;		// (9)�����ڵ�
} ;
#define		ONESIZE_BMASTER	 sizeof(20+9)

/* ��������,�����μ���  Master Layout */
class CBEMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;		// (20)�����
	CString m_szName;		// (9)�����ڵ�
} ;
#define		ONESIZE_BEMASTER	 sizeof(20+9)

#define		ONESIZE_BCMASTER	 sizeof(20+9)
/* �����μ�������  Master Layout */
class CBCMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);
	CString m_szCode;		// (20)�����
	CString m_szName;		// (9)�����ڵ�
} ;

class CSTKOPT	: public CMasterBase
{
public:
	virtual BOOL ReadData(LPCTSTR _szStr);
	CString m_szName;		// �����
	CString m_szCode;		// �ڵ�
	CString m_szATM;		 // ATM ����
	CString m_szBaseCode;	// �����ڻ� �ڵ�
};
#define		ONESIZE_STKOPT		37

// �ֽĿɼ� ������
#define	ONESIZE_STKDMASTER	6
class CSTKDMASTER : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szDate;		// 7 ������
};

// �ڼ��� 2002.10.09
//* ETF(���������ݵ�)
#define		ONESIZE_ETFMASTER		(20+6+1)
class CETFMaster : public CMasterBase
{
public:
	virtual BOOL ReadData(LPCSTR _szStr);

	CString m_szName;		// 20 �ڵ��
	CString	m_szCode;		//  6 �ڵ�
	CString m_szGubun;		//  1 �ŷ���1, �ڽ���2
};

//#define ONESIZE_BMASTER (12 + 9)
//class CBMaster : public CMasterBase
//{
//public:
//	virtual BOOL ReadData(LPCSTR _szStr);
//	
//	CString m_szName;		//12 �����
//	CString m_szCode;		// 9 �����ڵ�
//};

///* ä�� Master Layout */
//typedef struct _CMASTER {
//    char    name    [45];     /* ä�Ǹ� */
//    char    code    [9];      /* �ڵ� */
//} CMASTER;
//#define		ONESIZE_CMASTER	 sizeof(CMASTER)
//
///* KOFEX Master Layout */
//typedef struct _KFMASTER {
//    char    name    [30];     /* ����� */
//    char    code    [15];      /* �ڵ� */
//} KFMASTER;
//#define		ONESIZE_KFMASTER	 sizeof(KFMASTER)
//
///* KOFEXTS Master Layout */
//typedef struct _KFTSMASTER {
//    char    code    [15];      /* �ڵ� */
//} KFTSMASTER;
//#define		ONESIZE_KFTSMASTER	 sizeof(KFTSMASTER)
//
///* ��������,�����μ���  Master Layout */
//typedef struct _BEMASTER {
//    char    name   [20];     /* ����� */
//    char    code    [9];     /* �����ڵ� */
//} BEMASTER;
//#define		ONESIZE_BEMASTER	 sizeof(BEMASTER)

//������ �ϱ����� Ŭ����  2000.11.28 code by nykim
class CSortClass
{
public:
	CSortClass(CListCtrl * _pWnd, const int _iCol, const bool _bIsNumeric);
	virtual ~CSortClass();
	
	int iCol;
	CListCtrl * pWnd;
	bool bIsNumeric;
	void Sort(const bool bAsc);
	
	static int CALLBACK CompareAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
	static int CALLBACK CompareAscI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK CompareDesI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
public:
	class CSortItem
	{
	public:
		virtual  ~CSortItem();
		CSortItem(const DWORD _dw, const CString &_txt);
		CString txt;
		DWORD dw;
	};
	class CSortItemInt
	{
	public:
		CSortItemInt(const DWORD _dw, const CString &_txt);
		int iInt ;
		DWORD dw;
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__LOCALUTIL__HEADER__)
