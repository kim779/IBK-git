/*************************************************************************************/
/*  1. ���α׷�ID : KB_s3301                                                         */
/*  2. ���α׷��� : �ֽļ���(ȣ��)                                                   */
/*  3. ���񽺸�   : KB_s3301                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_s3301_H__
#define __KB_TRAN_KB_s3301_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_s3301"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		futcode[8];		/*  �����ڵ�     */
	char		mode[1];		/*  ��ȸ���     */
	char		count[4];		/*  ��ȸ�Ǽ�     */
}KB_s3301_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char KB_s3301_OutRec1_count[4];
//typedef struct
//{
//	char		futcode[8];		/*  �����ڵ�     */
//	char		expcode[12];		/*�����ڵ�     */
//	char		seqno[3];		/*  ����SEQ��ȣ     */
//	char		hname[50];		/*�ѱ������     */
//	char		ename[50];		/*���������     */
//	char		sname[25];		/*��������     */
//	char		name[4];		/*  �����     */
//	char		listdate[8];		/*  ������     */
//	char		futgubun[1];		/*  �ֱٿ�������     */
//	char		listdatecnt[3];		/*  �����ϼ�     */
//	char		jandatecnt[3];		/*  �����ϼ�     */
//	char		lastdate[8];		/*  �����ŷ���     */
//	char		lastdategubun[1];		/*  �����ŷ��Ͽ���     */
//	char		divideratio[13];		/*  ���������̷���ġ     */
//	char		recpricegubun[1];		/*  ���ذ��ݱ���     */
//	char		recprice[7];		/*  ���ذ���     */
//	char		theorypricek[13];		/*  �̷а�(���갡)     */
//	char		theorypriceg[13];		/*  �̷а�(���ذ�)     */
//	char		cdratio[6];		/*  CD�ݸ�     */
//	char		uplmtprice[7];		/*  ���Ѱ�     */
//	char		dnlmtprice[7];		/*  ���Ѱ�     */
//	char		jnilsetgubun[1];		/*  �������갡����     */
//	char		jnilsetprice[16];		/*  �������갡     */
//	char		jnilclosegubun[1];		/*  ������������     */
//	char		jnilclose[7];		/*  ��������     */
//	char		jnilvolume[8];		/*  ����ü�����     */
//	char		jnilgmvolume[8];		/*  ���ϱٿ���ü�����     */
//	char		jnilwmvolume[8];		/*  ���Ͽ�����ü�����     */
//	char		jnilvalue[15];		/*  ����ü����     */
//	char		jnilgmvalue[15];		/*  ���ϱٿ����ŷ����     */
//	char		jnilwmvalue[15];		/*  ���Ͽ������ŷ����     */
//	char		jnilopenyak[8];		/*  ���Ϲ̰�����������     */
//	char		listhprice[7];		/*  �����ְ�     */
//	char		listhdate[8];		/*  �����ְ���     */
//	char		listlprice[7];		/* ����������     */
//	char		listldate[8];		/*  ������������     */
//	char		ipwhedate[8];		/*  ��ȸ����     */
//	char		mrkgubun[1];		/*  ���尡��뱸��     */
//	char		jogengubun[1];		/*  ���Ǻ���������뱸��     */
//	char		choiygubun[1];		/*  ��������������뱸��     */
//	char		lastprice[16];		/*  ������������     */
//	char		lastpricegubun[1];		/*  �����������ݱ���     */
//	char		cbhprice[7];		/*  CB������Ѱ�     */
//	char		cblprice[7];		/*  CB�������Ѱ�     */
//	char		jnilmmchetime[8];		/*  ���������Ÿ�ü��ð�     */
//	char		jnilspgmclose[7];		/*  ���Ͻ�������ٿ�������     */
//	char		jnilspwmclose[7];		/*  ���Ͻ����������������     */
//	char		gexpcode[12];		/*  ��������ٿ���ǥ���ڵ�     */
//	char		wexpcode[12];		/*  �������������ǥ���ڵ�     */
//	char		tradeunit[16];		/*  �ŷ�����     */
//	char		mktmakegubun[1];		/*  �����������񿩺�     */
//	char		adjustgubun[1];		/*  ��������     */
//	char		stopgubun[1];		/*  �ŷ���������     */
//	char		settletype[1];		/*  �������     */
//	char		mmchetype[1];		/*  �Ÿ�ü����     */
//	char		itemcode[12];		/*  ����ֽ��ڵ�     */
//	char		expname[20];		/*  ǥ�������     */
//	char		bdate[8];		/*  ������������     */
//	char		bjandatecnt[3];		/*  �����������ϼ�     */
//	char		chetime[10];		/*  ü��ð�     */
//	char		price[7];		/*  ���簡(SIGN����)     */
//	char		open[7];		/*  �ð�(SIGN����)     */
//	char		high[7];		/*  ��(SIGN����)     */
//	char		low[7];		/*  ����(SIGN����)     */
//	char		sign[1];		/*  ���ϴ�񱸺�     */
//	char		change[7];		/*���ϴ��     */
//	char		volume[12];		/*�����ŷ���     */
//	char		value[15];		/*�����ŷ����     */
//	char		cvolume[6];		/*�Ǻ�ü�����     */
//	char		cgubun[1];		/*ü�ᱸ��     */
//	char		mdvolume[7];		/*  �ŵ������ŷ���     */
//	char		msvolume[7];		/*  �ż������ŷ���     */
//	char		mdchecnt[6];		/*  �ŵ�����ü��Ǽ�     */
//	char		mschecnt[6];		/*  �ż�����ü��Ǽ�     */
//	char		gmprice[7];		/*  �ٿ���������������     */
//	char		wmprice[7];		/*  ������������������     */
//	char		gmvolume[10];		/*  �ٿ���ü�����     */
//	char		wmvolume[10];		/*  ������ü�����     */
//	char		gmvalue[15];		/*  �ٿ����ŷ����     */
//	char		wmvalue[15];		/*  �������ŷ����     */
//	char		mdvolume_h[10];		/*  �ŵ�ü�����(ȣ������)     */
//	char		msvolume_h[10];		/*  �ż�ü�����(ȣ������)     */
//	char		hotime[10];		/*  ȣ�����Žð�     */
//	char		offerho1[8];		/*�ŵ�ȣ��1     */
//	char		bidho1[8];		/*  �ż�ȣ��1     */
//	char		offerrem1[6];		/*�ŵ�ȣ������1     */
//	char		bidrem1[6];		/*  �ż�ȣ������1     */
//	char		offercnt1[4];		/*�ŵ�ȣ���Ǽ�1     */
//	char		bidcnt1[4];		/*  �ż�ȣ���Ǽ�1     */
//	char		offerremcha1[6];		/*�����ŵ�������1     */
//	char		bidremcha1[6];		/*  �����ż�������1     */
//	char		offercntcha1[4];		/*�����ŵ����Ǽ�1     */
//	char		bidcntcha1[4];		/*  �����ż����Ǽ�1     */
//	char		offerho2[8];		/*�ŵ�ȣ��2     */
//	char		bidho2[8];		/*  �ż�ȣ��2     */
//	char		offerrem2[6];		/*�ŵ�ȣ������2     */
//	char		bidrem2[6];		/*  �ż�ȣ������2     */
//	char		offercnt2[4];		/*�ŵ�ȣ���Ǽ�2     */
//	char		bidcnt2[4];		/*  �ż�ȣ���Ǽ�2     */
//	char		offerremcha2[6];		/*�����ŵ�������2     */
//	char		bidremcha2[6];		/*  �����ż�������2     */
//	char		offercntcha2[4];		/*�����ŵ����Ǽ�2     */
//	char		bidcntcha2[4];		/*  �����ż����Ǽ�2     */
//	char		offerho3[8];		/*�ŵ�ȣ��3     */
//	char		bidho3[8];		/*  �ż�ȣ��3     */
//	char		offerrem3[6];		/*�ŵ�ȣ������3     */
//	char		bidrem3[6];		/*  �ż�ȣ������3     */
//	char		offercnt3[4];		/*�ŵ�ȣ���Ǽ�3     */
//	char		bidcnt3[4];		/*  �ż�ȣ���Ǽ�3     */
//	char		offerremcha3[6];		/*�����ŵ�������3     */
//	char		bidremcha3[6];		/*  �����ż�������3     */
//	char		offercntcha3[4];		/*�����ŵ����Ǽ�3     */
//	char		bidcntcha3[4];		/*  �����ż����Ǽ�3     */
//	char		offerho4[8];		/*�ŵ�ȣ��4     */
//	char		bidho4[8];		/*  �ż�ȣ��4     */
//	char		offerrem4[6];		/*�ŵ�ȣ������4     */
//	char		bidrem4[6];		/*  �ż�ȣ������4     */
//	char		offercnt4[4];		/*�ŵ�ȣ���Ǽ�4     */
//	char		bidcnt4[4];		/*  �ż�ȣ���Ǽ�4     */
//	char		offerremcha4[6];		/*�����ŵ�������4     */
//	char		bidremcha4[6];		/*  �����ż�������4     */
//	char		offercntcha4[4];		/*�����ŵ����Ǽ�4     */
//	char		bidcntcha4[4];		/*  �����ż����Ǽ�4     */
//	char		offerho5[8];		/*�ŵ�ȣ��5     */
//	char		bidho5[8];		/*  �ż�ȣ��5     */
//	char		offerrem5[6];		/*�ŵ�ȣ������5     */
//	char		bidrem5[6];		/*  �ż�ȣ������5     */
//	char		offercnt5[4];		/*�ŵ�ȣ���Ǽ�5     */
//	char		bidcnt5[4];		/*  �ż�ȣ���Ǽ�5     */
//	char		offerremcha5[6];		/*�����ŵ�������5     */
//	char		bidremcha5[6];		/*  �����ż�������5     */
//	char		offercntcha5[4];		/*�����ŵ����Ǽ�5     */
//	char		bidcntcha5[4];		/*  �����ż����Ǽ�5     */
//	char		offerho6[8];		/*�ŵ�ȣ��6     */
//	char		bidho6[8];		/*  �ż�ȣ��6     */
//	char		offerrem6[6];		/*�ŵ�ȣ������6     */
//	char		bidrem6[6];		/*  �ż�ȣ������6     */
//	char		offercnt6[4];		/*�ŵ�ȣ���Ǽ�6     */
//	char		bidcnt6[4];		/*  �ż�ȣ���Ǽ�6     */
//	char		offerremcha6[6];		/*�����ŵ�������6     */
//	char		bidremcha6[6];		/*  �����ż�������6     */
//	char		offercntcha6[4];		/*�����ŵ����Ǽ�6     */
//	char		bidcntcha6[4];		/*  �����ż����Ǽ�6     */
//	char		offerho7[8];		/*�ŵ�ȣ��7     */
//	char		bidho7[8];		/*  �ż�ȣ��7     */
//	char		offerrem7[6];		/*�ŵ�ȣ������7     */
//	char		bidrem7[6];		/*  �ż�ȣ������7     */
//	char		offercnt7[4];		/*�ŵ�ȣ���Ǽ�7     */
//	char		bidcnt7[4];		/*  �ż�ȣ���Ǽ�7     */
//	char		offerremcha7[6];		/*�����ŵ�������7     */
//	char		bidremcha7[6];		/*  �����ż�������7     */
//	char		offercntcha7[4];		/*�����ŵ����Ǽ�7     */
//	char		bidcntcha7[4];		/*  �����ż����Ǽ�7     */
//	char		offerho8[8];		/*�ŵ�ȣ��8     */
//	char		bidho8[8];		/*  �ż�ȣ��8     */
//	char		offerrem8[6];		/*�ŵ�ȣ������8     */
//	char		bidrem8[6];		/*  �ż�ȣ������8     */
//	char		offercnt8[4];		/*�ŵ�ȣ���Ǽ�8     */
//	char		bidcnt8[4];		/*  �ż�ȣ���Ǽ�8     */
//	char		offerremcha8[6];		/*�����ŵ�������8     */
//	char		bidremcha8[6];		/*  �����ż�������8     */
//	char		offercntcha8[4];		/*�����ŵ����Ǽ�8     */
//	char		bidcntcha8[4];		/*  �����ż����Ǽ�8     */
//	char		offerho9[8];		/*�ŵ�ȣ��9     */
//	char		bidho9[8];		/*  �ż�ȣ��9     */
//	char		offerrem9[6];		/*�ŵ�ȣ������9     */
//	char		bidrem9[6];		/*  �ż�ȣ������9     */
//	char		offercnt9[4];		/*�ŵ�ȣ���Ǽ�9     */
//	char		bidcnt9[4];		/*  �ż�ȣ���Ǽ�9     */
//	char		offerremcha9[6];		/*�����ŵ�������9     */
//	char		bidremcha9[6];		/*  �����ż�������9     */
//	char		offercntcha9[4];		/*�����ŵ����Ǽ�9     */
//	char		bidcntcha9[4];		/*  �����ż����Ǽ�9     */
//	char		offerho10[8];		/*�ŵ�ȣ��10     */
//	char		bidho10[8];		/*  �ż�ȣ��10     */
//	char		offerrem10[6];		/*�ŵ�ȣ������10     */
//	char		bidrem10[6];		/*  �ż�ȣ������10     */
//	char		offercnt10[4];		/*�ŵ�ȣ���Ǽ�10     */
//	char		bidcnt10[4];		/*  �ż�ȣ���Ǽ�10     */
//	char		offerremcha10[6];		/*�����ŵ�������10     */
//	char		bidremcha10[6];		/*  �����ż�������10     */
//	char		offercntcha10[4];		/*�����ŵ����Ǽ�10     */
//	char		bidcntcha10[4];		/*  �����ż����Ǽ�10     */
//	char		totofferrem[7];		/*�ŵ�ȣ���Ѽ���     */
//	char		totbidrem[7];		/*  �ż�ȣ���Ѽ���     */
//	char		totoffercnt[5];		/*�ŵ�ȣ���ѰǼ�     */
//	char		totbidcnt[5];		/*  �ż�ȣ���ѰǼ�     */
//	char		totofferremcha[7];		/*�����ŵ�����Ѽ���     */
//	char		totbidremcha[7];		/*  �����ż�����Ѽ���     */
//	char		totoffercntcha[5];		/*�����ŵ�����ѰǼ�     */
//	char		totbidcntcha[5];		/*  �����ż�����ѰǼ�     */
//	char		jgubun[2];		/*  ���뱸��     */
//	char		yakdate[8];		/*  ü������     */
//	char		mggubun[1];		/*  �̰�����������     */
//	char		openyak[7];		/*  �̰�����������     */
//	char		openyakcha[7];		/*  �̰������������������     */
//	char		openopenyak[7];		/*  �̰��������ð�     */
//	char		highopenyak[7];		/*  �̰���������     */
//	char		lowopenyak[7];		/*  �̰�����������     */
//	char		jsprice[16];		/*���갡��     */
//	char		jsgubun[1];		/*  ���갡����     */
//	char		baseprice[6];		/*  ����������簡     */
//	char		theoryprice[6];		/*  �̷а�(���簡)     */
//	char		kasis[6];		/*  ������     */
//	char		sbasis[6];		/*  ����BASIS     */
//	char		ibasis[6];		/*  �̷�BASIS     */
//	char		drate[6];		/*  �����     */
//	char		b_volrate[8];		/*  ���ϰŷ�����     */
//	char		svolume[8];		/*  ȣ�����ż�     */
//	char		pivot[8];		/*  �Ǻ�     */
//	char		resist1[8];		/*  1������     */
//	char		support1[8];		/*1������     */
//	char		resist2[8];		/*  2������     */
//	char		support2[8];		/*  2������     */
//	char		ho_drate1[6];		/*  �ŵ�ȣ�������1     */
//	char		ho_drate2[6];		/*  �ŵ�ȣ�������2     */
//	char		ho_drate3[6];		/*  �ŵ�ȣ�������3     */
//	char		ho_drate4[6];		/*  �ŵ�ȣ�������4     */
//	char		ho_drate5[6];		/*  �ŵ�ȣ�������5     */
//	char		ho_drate6[6];		/*  �ŵ�ȣ�������6     */
//	char		ho_drate7[6];		/*  �ŵ�ȣ�������7     */
//	char		ho_drate8[6];		/*  �ŵ�ȣ�������8     */
//	char		ho_drate9[6];		/*  �ŵ�ȣ�������9     */
//	char		ho_drate10[6];		/*�ŵ�ȣ�������10     */
//	char		hb_drate1[6];		/*  �ż�ȣ�������1     */
//	char		hb_drate2[6];		/*  �ż�ȣ�������2     */
//	char		hb_drate3[6];		/*  �ż�ȣ�������3     */
//	char		hb_drate4[6];		/*  �ż�ȣ�������4     */
//	char		hb_drate5[6];		/*  �ż�ȣ�������5     */
//	char		hb_drate6[6];		/*  �ż�ȣ�������6     */
//	char		hb_drate7[6];		/*  �ż�ȣ�������7     */
//	char		hb_drate8[6];		/*  �ż�ȣ�������8     */
//	char		hb_drate9[6];		/*  �ż�ȣ�������9     */
//	char		hb_drate10[6];		/*  �ż�ȣ�������10     */
//}KB_s3301_OutRec1;

typedef struct _KB_s3301_OutRec1 {
	char		futcode[8];		/*  �����ڵ�     */
	char		recprice[7];		/*  ���ذ���     */
	char		uplmtprice[7];		/*  ���Ѱ�     */
	char		dnlmtprice[7];		/*  ���Ѱ�     */
	char		price[7];		/*  ���簡     */
	char		offerho1[8];		/*  �ŵ�ȣ��1     */
	char		offerho2[8];		/*  �ŵ�ȣ��2     */
	char		offerho3[8];		/*  �ŵ�ȣ��3     */
	char		bidho1[8];		/*  �ż�ȣ��1     */
	char		bidho2[8];		/*  �ż�ȣ��2     */
	char		bidho3[8];		/*  �ż�ȣ��3     */
} KB_s3301_OutRec1;

/*************************************************************************************/
class CKB_s3301
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
	CKB_s3301()
	{
	}

	virtual void  Convert(KB_s3301_OutRec1* pInput)
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

