#if !defined(AFX_EUREAL_SFC0__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_)
#define AFX_EUREAL_SFC0__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     ��Ÿ
//--------------------------------------------------------------------

typedef struct {
    char    dgubun            [4]; /* Data ���� : 'SFC0'                     */
    char    kfutcode          [8]; /* ���������ڵ�                           */
    char    chetime           [6]; /* ü��ð� HHMMSS                        */
    char    sign              [1]; /* 2:��� 3:���� 5:�϶�                   */
    char    change            [6]; /* ���ϴ��                     9(4)v9(2) */
    char    drate             [6]; /* �����                       9(4)v9(2) */
    char    price             [6]; /* ���簡                       9(4)v9(2) */
    char    open              [6]; /* �ð�                         9(4)v9(2) */
    char    high              [6]; /* ��                         9(4)v9(2) */
    char    low               [6]; /* ����                         9(4)v9(2) */
    char    cgubun            [1]; /* ü�ᱸ��           ('+':�ż� '-':�ż�) */
    char    cvolume           [6]; /* ü�ᷮ                       ����:��� */
    char    volume           [12]; /* �����ŷ���                   ����:��� */
    char    volchange        [12]; /* �ŷ������ϴ��               ����:��� */
    char    voldrate          [8]; /* �ŷ��������                 9(6)v9(2) */
    char    value            [12]; /* �����ŷ����                 ����:õ�� */
    char    mdvolume         [12]; /* �ŵ����� ü�ᷮ                        */
    char    mdchecnt          [8]; /* �ŵ����� ü��Ǽ�                      */
    char    msvolume         [12]; /* �ż����� ü�ᷮ                        */
    char    mschecnt          [8]; /* �ż����� ü��Ǽ�                      */
    char    cpower            [9]; /* ü�ᰭ�� 999999.99                     */
    char    offerho1          [6]; /* �ŵ�ȣ��1                    9(4)v9(2) */
    char    bidho1            [6]; /* �ż�ȣ��1                    9(4)v9(2) */
    char    openyak           [8]; /* �̰�����������               ����:��� */
    char    openyakchange     [8]; /* �̰����������ϴ��           ����:��� */
    char    starjisu          [6]; /* ��Ÿ����                               */
    char    theoryprice       [6]; /* �̷а�                       9(4)v9(2) */
    char    grate             [6]; /* ������                       9(4)v9(2) */
    char    kasis             [6]; /* ������                       9(4)v9(2) */
    char    sbasis            [6]; /* ���� BASIS                   9(4)v9(2) */
    char    ibasis            [6]; /* �̷� BASIS                   9(4)v9(2) */
    char    futcode           [8]; /* ���������ڵ�                           */
} stEUREAL_SFC0;	//67

class CEUREAL_SFC0
{
public: //Variable
    CString m_szdgubun;			/* Data ���� : 'SFC0'                     */
    CString m_szkfutcode;		/* ���������ڵ�                           */
    CString m_szchetime;		/* ü��ð� HHMMSS                        */
    CString m_szsign;			/* 2:��� 3:���� 5:�϶�                   */
    CString m_szchange;			/* ���ϴ��                     9(4)v9(2) */
    CString m_szdrate;			/* �����                       9(4)v9(2) */
    CString m_szprice;			/* ���簡                       9(4)v9(2) */
    CString m_szopen;			/* �ð�                         9(4)v9(2) */
    CString m_szhigh;			/* ��                         9(4)v9(2) */
    CString m_szlow;			/* ����                         9(4)v9(2) */
    CString m_szcgubun;			/* ü�ᱸ��           ('+':�ż� '-':�ż�) */
    CString m_szcvolume;		/* ü�ᷮ                       ����:��� */
    CString m_szvolume;			/* �����ŷ���                   ����:��� */
    CString m_szvolchange;		/* �ŷ������ϴ��               ����:��� */
    CString m_szvoldrate;		/* �ŷ��������                 9(6)v9(2) */
    CString m_szvalue;			/* �����ŷ����                 ����:õ�� */
    CString m_szmdvolume;		/* �ŵ����� ü�ᷮ                        */
    CString m_szmdchecnt;		/* �ŵ����� ü��Ǽ�                      */
    CString m_szmsvolume;		/* �ż����� ü�ᷮ                        */
    CString m_szmschecnt;		/* �ż����� ü��Ǽ�                      */
    CString m_szcpower;			/* ü�ᰭ�� 999999.99                     */
    CString m_szofferho1;		/* �ŵ�ȣ��1                    9(4)v9(2) */
    CString m_szbidho1;			/* �ż�ȣ��1                    9(4)v9(2) */
    CString m_szopenyak;		/* �̰�����������               ����:��� */
    CString m_szopenyakchange;	/* �̰����������ϴ��           ����:��� */
    CString m_szstarjisu;		/* ��Ÿ����                               */
    CString m_sztheoryprice;	/* �̷а�                       9(4)v9(2) */
    CString m_szgrate;			/* ������                       9(4)v9(2) */
    CString m_szkasis;			/* ������                       9(4)v9(2) */
    CString m_szsbasis;			/* ���� BASIS                   9(4)v9(2) */
    CString m_szibasis;			/* �̷� BASIS                   9(4)v9(2) */
    CString m_szfutcode;		/* ���������ڵ�                           */

public:
	CEUREAL_SFC0()
	{
	}

	virtual void  Convert(stEUREAL_SFC0* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun            );/* Data ���� : 'SFC0'                     */
		DATA_MEMCOPY(sTmp, pInput, kfutcode          );/* ���������ڵ�                           */
		DATA_MEMCOPY(sTmp, pInput, chetime           );/* ü��ð� HHMMSS                        */
		DATA_MEMCOPY(sTmp, pInput, sign              );/* 2:��� 3:���� 5:�϶�                   */
		DATA_MEMCOPY(sTmp, pInput, change            );/* ���ϴ��                     9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, drate             );/* �����                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, price             );/* ���簡                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, open              );/* �ð�                         9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, high              );/* ��                         9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, low               );/* ����                         9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, cgubun            );/* ü�ᱸ��           ('+':�ż� '-':�ż�) */
		DATA_MEMCOPY(sTmp, pInput, cvolume           );/* ü�ᷮ                       ����:��� */
		DATA_MEMCOPY(sTmp, pInput, volume            );/* �����ŷ���                   ����:��� */
		DATA_MEMCOPY(sTmp, pInput, volchange         );/* �ŷ������ϴ��               ����:��� */
		DATA_MEMCOPY(sTmp, pInput, voldrate          );/* �ŷ��������                 9(6)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, value             );/* �����ŷ����                 ����:õ�� */
		DATA_MEMCOPY(sTmp, pInput, mdvolume          );/* �ŵ����� ü�ᷮ                        */
		DATA_MEMCOPY(sTmp, pInput, mdchecnt          );/* �ŵ����� ü��Ǽ�                      */
		DATA_MEMCOPY(sTmp, pInput, msvolume          );/* �ż����� ü�ᷮ                        */
		DATA_MEMCOPY(sTmp, pInput, mschecnt          );/* �ż����� ü��Ǽ�                      */
		DATA_MEMCOPY(sTmp, pInput, cpower            );/* ü�ᰭ�� 999999.99                     */
		DATA_MEMCOPY(sTmp, pInput, offerho1          );/* �ŵ�ȣ��1                    9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, bidho1            );/* �ż�ȣ��1                    9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, openyak           );/* �̰�����������               ����:��� */
		DATA_MEMCOPY(sTmp, pInput, openyakchange     );/* �̰����������ϴ��           ����:��� */
		DATA_MEMCOPY(sTmp, pInput, starjisu          );/* ��Ÿ����                               */
		DATA_MEMCOPY(sTmp, pInput, theoryprice       );/* �̷а�                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, grate             );/* ������                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, kasis             );/* ������                       9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, sbasis            );/* ���� BASIS                   9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, ibasis            );/* �̷� BASIS                   9(4)v9(2) */
		DATA_MEMCOPY(sTmp, pInput, futcode           );/* ���������ڵ�                           */
	}
};

#endif // AFX_EUREAL_SFC0__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_
