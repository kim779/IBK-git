/*************************************************************************************/
/*  1. ���α׷�ID : KB_f0201                                                         */
/*  2. ���α׷��� : �����ɼ�(ȣ��)                                                   */
/*  3. ���񽺸�   : KB_f0201                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_f0201_H__
#define __KB_TRAN_KB_f0201_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_f0201"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		futcode[8];		/*  �����ڵ�     */
//	char		mode[1];		/*  ��ȸ���     */
//	char		count[4];		/*  ��ȸ�Ǽ�     */
}KB_f0201_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

typedef struct _KB_f0201_OutRec1 {
	char		futcode[8];		/*  �����ڵ�     */
	char		recprice[6];		/*  ���ذ���     */
	char		uplmtprice[6];		/*  ���Ѱ�     */
	char		dnlmtprice[6];		/*  ���Ѱ�     */
	char		price[6];		/*  ���簡     */
	char		offerho1[6];		/*  �ŵ�ȣ��1     */
	char		offerho2[6];		/*  �ŵ�ȣ��2     */
	char		offerho3[6];		/*  �ŵ�ȣ��3     */
	char		bidho1[6];		/*  �ż�ȣ��1     */
	char		bidho2[6];		/*  �ż�ȣ��2     */
	char		bidho3[6];		/*  �ż�ȣ��3     */
} KB_f0201_OutRec1;

/*************************************************************************************/
class CKB_f0201
{
public: //Variable
	CString		m_szfutcode;		/*  �����ڵ�     */
	CString		m_szrecprice;		/*  ���ذ���     */
	CString		m_szuplmtprice;		/*  ���Ѱ�     */
	CString		m_szdnlmtprice;		/*  ���Ѱ�     */
	CString		m_szprice;		/*  ���簡     */
	CString		m_szofferho1;		/*  �ŵ�ȣ��1     */
	CString		m_szofferho2;		/*  �ŵ�ȣ��2     */
	CString		m_szofferho3;		/*  �ŵ�ȣ��3     */
	CString		m_szbidho1;		/*  �ż�ȣ��1     */
	CString		m_szbidho2;		/*  �ż�ȣ��2     */
	CString		m_szbidho3;		/*  �ż�ȣ��3     */

public:
	CKB_f0201()
	{
	}

	virtual void  Convert(KB_f0201_OutRec1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, futcode);		/*  �����ڵ�     */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/*  ���ذ���     */
		DATA_MEMCOPY(sTmp, pInput, uplmtprice);		/*  ���Ѱ�     */
		DATA_MEMCOPY(sTmp, pInput, dnlmtprice);		/*  ���Ѱ�     */
		DATA_MEMCOPY(sTmp, pInput, price);		/*  ���簡     */
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  �ŵ�ȣ��1     */
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/*  �ŵ�ȣ��2     */
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/*  �ŵ�ȣ��3     */
		DATA_MEMCOPY(sTmp, pInput, bidho1);		/*  �ż�ȣ��1     */
		DATA_MEMCOPY(sTmp, pInput, bidho2);		/*  �ż�ȣ��2     */
		DATA_MEMCOPY(sTmp, pInput, bidho3);		/*  �ż�ȣ��3     */
	}
};

#endif

