#ifndef _DEF_GWANSIM_DEFINE__HEADER__
#define _DEF_GWANSIM_DEFINE__HEADER__

// #include "../../inc/Define_Gwansim.h"
//#include "../Dlls/CommonTR/Data_Common.h"
#include "../chart/CommonTR/Data_Common.h"


const UINT RMSG_CONCERNHWND = ::RegisterWindowMessage(_T("RMSG_CONCERNHWND") );
const UINT RMSG_INTREQUEST = ::RegisterWindowMessage(_T("RMSG_INTREQUEST") );

//{{ ����TR��ȸ����.
#define TR_88803				_T("88803")
#define REQUEST_INTTOTAL		"01" // ���� ��� ���� ���(�޸� ī���ؾ� ��) 
#define GWAMSIM_DEF_NAME		"���ɱ׷�"
//}} ����


typedef struct
{
	char	Igubun[2]	;	// 0 : ERROR, 1 : 1��ü ����(���񸮽�Ʈ ����), 2 : ���� �׷� ���, 3 : ���� ���    
	char	Icnt[3]		;	// �׷� ��
	char	IopenDlg[1]	;	// 0 : Open, 1 : No Open
	void*	Idata		;	// �׷� �����͵�(CPtrArray) 
	char	Ititle[256]	;
}INT_TOTAL;
#define		SIZE_INT_TOTAL	sizeof(INT_TOTAL)

typedef struct
{
	char	Ino		[ 2];	// �׷� ��ȣ 
	char	Ititle	[20];	// �׷� ��
	char	Icnt	[ 3];	// �׷� �ȿ� ���� ��
	void*	Idata		;	// ���� �����͵�(CPtrArray)
	char	Itime	[ 6];
	char	Idate	[ 8];
}INT_GROUP_ST;
#define		SIZE_INT_GROUP_ST	sizeof(INT_GROUP_ST)

typedef struct
{
	char Icode[12];			// �ڵ�  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   ��,��,��
	char Idate[8];		   // YYYYMMDD ��,��,��
	char Ivol[10];		   // ���� 	
	char Ival[10];		   // ���԰���
	char ICheck[1];		   // üũ����
	void* Idata;
	char IName[30];
} INT_ITEM_ST;
#define		SIZE_INT_ITEM_ST	sizeof(INT_ITEM_ST)

class CINT_TOTAL
{
public:
	CString  m_szIgubun		;	// 0 : ERROR, 1 : 1��ü ����(���񸮽�Ʈ ����), 2 : ���� �׷� ���, 3 : ���� ���    
	CString  m_szIcnt		;	// �׷� ��
	CString  m_szIopenDlg	;	// 0 : Open, 1 : No Open
	void*	 m_pIdata		;	// �׷� �����͵�(CPtrArray) 
	CString  m_szItitle		;

public:
	CINT_TOTAL()
	{
	}

	CINT_TOTAL(INT_TOTAL* pItem)
	{
		Convert(pItem);
	}

	void Convert(INT_TOTAL* pItem)
	{
		char sTmp[256+1]={0,};

		DATA_MEMCOPY(sTmp, pItem, Igubun);
		DATA_MEMCOPY(sTmp, pItem, Icnt);
//		DATA_MEMCOPY(sTmp, pItem, IopenDlg);
		DATA_MEMCOPY(sTmp, pItem, Ititle);

		m_pIdata = pItem->Idata;
	}
};

class CINT_GROUP_ST
{
public:
	CString  m_szIno	;	// �׷� ��ȣ 
	CString  m_szItitle	;	// �׷� ��
	CString  m_szIcnt	;	// �׷� �ȿ� ���� ��
	void*	 m_pIdata	;	// ���� �����͵�(CPtrArray)
	CString  m_szItime	;
	CString  m_szIdate	;

	int      m_nIcnt;
	
public:
	CINT_GROUP_ST()
	{
		m_nIcnt = 0;
	}

	CINT_GROUP_ST(INT_GROUP_ST* pItem)
	{
		Convert(pItem);
	}

	void Convert(INT_GROUP_ST* pItem)
	{
		char sTmp[21+1]={0,};

		DATA_MEMCOPY(sTmp, pItem, Ino);
		DATA_MEMCOPY(sTmp, pItem, Ititle);
		DATA_MEMCOPY(sTmp, pItem, Icnt);
//		DATA_MEMCOPY(sTmp, pItem, Itime);
//		DATA_MEMCOPY(sTmp, pItem, Idate);

		m_pIdata = pItem->Idata;

		m_nIcnt = atoi(m_szIcnt);
	}

	void LocalTRACE(LPCSTR szMsg)
	{
		TRACE("%s\n", szMsg);
//		TRACE("�׷� ��ȣ [%s]\n", m_szIno);
//		TRACE("�׷� ��   [%s]\n", m_szIcnt);
//		TRACE("�ð�      [%s]\n", m_szItime);
//		TRACE("����      [%s]\n", m_szIdate);
		TRACE("--------------------------\n");
	}

};


class CINT_ITEM_ST
{
public:
	CString  m_szIcode;			// �ڵ�  
	CString  m_szIgubun;		// 0:Kospi, 1:kosdoq,,,
	CString  m_szItime;			// HHMMSS   ��,��,��
	CString  m_szIdate;			// YYYYMMDD ��,��,��
	CString  m_szIvol;			// ���� 	
	CString  m_szIval;			// ���԰���
	CString  m_szICheck;		// üũ����
	void*    m_pIdata;
	CString  m_szIName;

public:
	CINT_ITEM_ST()
	{
	}

	CINT_ITEM_ST(INT_ITEM_ST* pItem)
	{
		Convert(pItem);
	}

	void Convert(INT_ITEM_ST* pItem)
	{
		char sTmp[30+1]={0,};

		DATA_MEMCOPY(sTmp, pItem, Icode);		// �ڵ�  
		DATA_MEMCOPY(sTmp, pItem, Igubun);		// 0:Kospi, 1:kosdoq,,,
//		DATA_MEMCOPY(sTmp, pItem, Itime );		// HHMMSS   ��,��,��
//		DATA_MEMCOPY(sTmp, pItem, Idate	);		// YYYYMMDD ��,��,��
//		DATA_MEMCOPY(sTmp, pItem, Ivol	);		// ���� 	
//		DATA_MEMCOPY(sTmp, pItem, Ival	);		// ���԰���
//		DATA_MEMCOPY(sTmp, pItem, ICheck);		// üũ����
		DATA_MEMCOPY(sTmp, pItem, IName	);

		m_pIdata = pItem->Idata;
	}

	// TR88803������ ���� ����...'1':�ֽ�, '2':�ڽ���
	char GetMyGb()
	{
		if(m_szIgubun=="0") return '2';
		else if(m_szIgubun=="1") return '1'; 
		else return 'E';
	}

	void LocalTRACE(LPCSTR szMsg)
	{
		TRACE("%s\n", szMsg);
//
//		TRACE("�ڵ� [%s]\n", m_szIcode);		// �ڵ�  
//		TRACE("���� [%s]\n", m_szIgubun);		// 0:Kospi, 1:kosdoq,,,
//		TRACE("�ð� [%s]\n", m_szItime );		// HHMMSS   ��,��,��
//		TRACE("���� [%s]\n", m_szIdate	);		// YYYYMMDD ��,��,��
//		TRACE("���� [%s]\n", m_szIvol	);		// ���� 	
//		TRACE("���԰��� [%s]\n", m_szIval	);	// ���԰���
//		TRACE("üũ���� [%s]\n", m_szICheck);	// üũ����
//		TRACE("����� [%s]\n", m_szIName	);
		TRACE("--------------------------\n");
	}
};
#endif //_DEF_GWANSIM_DEFINE__HEADER__
