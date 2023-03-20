#ifndef __OAQ69901_H__
#define __OAQ69901_H__

#pragma pack(push)
#pragma pack(1)

#define	MIGYULJE_PID			0x61			// �̰��� �ǽð�

#define	TROAQ69901		"OAQ69901"

typedef struct tagOAQ69901_InRec
{
	char		userid[8];		/* USERID */
}OAQ69901_INREC;

typedef struct tagOutRec1
{
	char		acntno[10];		/* ���¹�ȣ */
	char		series[30];		/* ���� */
	char		buysellsect[1];	/* ���� */
	char		posqty[8];		/* �̰������� */
	char		posprice[12];	/* ��վ����� */
}OAQ69901_OUTREC1;

typedef struct tagOAQ69901_OutRec2
{
	char		acntno[10];		/* ���¹�ȣ */
	char		series[30];		/* ���� */
	char		ordno[5];		/* �ֹ���ȣ */
	char		buysellsect[1];	/* ���� */
	char		ordqty[8];		/* �ֹ��� */
	char		ordprice[12];	/* �ֹ��� */
}OAQ69901_OUTREC2;

//������ 0x61��.

typedef struct _Order_Info
{
	CString		m_stracntno;		/* ���¹�ȣ */
	CString		m_strseries;		/* ���� */
	CString		m_strbuysellsect;	/* ���� */
	CString		m_strposqty;		/* �̰������� */
	CString		m_strposprice;	/* ��վ����� */
	CString		m_strstate;		/* ó������ */
	double		m_dblPrice;		/* ��վ����� */
} ORDER_INFO;

#pragma pack(pop)

#endif // __OAQ69901_H__
