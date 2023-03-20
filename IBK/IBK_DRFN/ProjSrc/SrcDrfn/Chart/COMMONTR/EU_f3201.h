/*************************************************************************************/
/*  1. ���α׷�ID : EU_f3201                                                         */
/*  2. ���α׷��� : �ֽĿɼ�(ȣ��)                                                   */
/*  3. ���񽺸�   : EU_f3201                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_f3201_H__
#define __EU_TRAN_EU_f3201_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define EU_TRAN                       "EU_f3201"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char		optcode[8];		/*  �����ڵ�     */
	char		mode[1];		/*  ��ȸ���     */
	char		count[4];		/*  ��ȸ�Ǽ�     */
}EU_f3201_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char EU_f3201_OutRec1_count[4];
//typedef struct
//{
//	char		optcode[8];		/*  �����ڵ�     */
//	char		expcode[12];		/*  �����ڵ�     */
//	char		ipwhedate[8];		/*  ��ȸ����     */
//	char		seqno[4];		/*  ����SEQ��ȣ     */
//	char		opkind[1];		/* �ɼ�����     */
//	char		eitem[2];		/*  �ŷ��������     */
//	char		lastmonth[6];		/*  ������     */
//	char		actprice[15];		/*  ��簡��     */
//	char		actprice_r[8];		/*  ��簡��(��ȸ��)     */
//	char		tradeunit[16];		/*  �ŷ�����     */
//	char		queryno[4];		/*  ��ȸ����     */
//	char		hname[50];		/*  �ѱ������     */
//	char		ename[50];		/*  ���������     */
//	char		name[25];		/*  ��������     */
//	char		actgubun[1];		/*  �Ǹ������������     */
//	char		listdatecnt[4];		/*  �����ϼ�(�����ϼ�)     */
//	char		jandatecnt[4];		/*  �����ϼ�     */
//	char		optgubun[1];		/*  �ֱٿ�������     */
//	char		atmgubun[1];		/*  ATM����     */
//	char		listdate[8];		/*  ������     */
//	char		startdate[8];		/*  ���ʰŷ���     */
//	char		lastdate[8];		/*  �����ŷ���     */
//	char		lastdategubun[1];		/*  �����ŷ��ϱ���     */
//	char		newgubun[1];		/*  ������屸��     */
//	char		newdate[8];		/*  ��������۾���     */
//	char		divideratio[13];		/*  �����������簡ġ     */
//	char		theorypricek[13];		/*  �̷а�     */
//	char		jnilopen[7];		/*  ���Ͻð�     */
//	char		jnilhigh[7];		/*  ���ϰ�     */
//	char		jnillow[7];		/*  ��������     */
//	char		jnilclose[7];		/*  ��������     */
//	char		jnilclosegubun[1];		/*  ������������     */
//	char		bmarginprice[7];		/*  �Ÿ����űݱ��ذ�     */
//	char		bmargingubun[1];		/*  �Ÿ����űݱ��ذ�����     */
//	char		hprice[7];		/*  ����ȣ������     */
//	char		lprice[7];		/*  ����ȣ������     */
//	char		cdratio[6];		/*  CD�ݸ�     */
//	char		jnilopenyak[8];		/* ���Ϲ̰�����������     */
//	char		jnilvolume[8];		/*  ���ϰŷ���     */
//	char		jnilvalue[15];		/* ���ϰŷ����     */
//	char		yearhdate[8];		/*  �����ְ���     */
//	char		yearhprice[7];		/*  �����ְ�     */
//	char		yearldate[8];		/*  ������������     */
//	char		yearlprice[7];		/*  ����������     */
//	char		listhdate[8];		/*  �������ְ���     */
//	char		listhprice[7];		/*  �������ְ�     */
//	char		listldate[8];		/*  ��������������     */
//	char		listlprice[7];		/*  ������������     */
//	char		mrkgubun[1];		/*  ���尡��뱸��     */
//	char		jogengubun[1];		/*  ���Ǻ���������뱸��     */
//	char		choiygubun[1];		/*  ��������������뱸��     */
//	char		recprice[7];		/* ���ذ���     */
//	char		recpricegubun[1];		/*  ���ذ��ݱ���     */
//	char		bimpv[6];		/* ���纯����     */
//	char		adjustgubun[1];		/*  ��������     */
//	char		stopgubun[1];		/*  �ŷ���������     */
//	char		mmchetype[1];		/*  �ŷ���������     */
//	char		itemcode[12];		/*  �����ڻ������ڵ�     */
//	char		settletype[1];		/*  �������     */
//	char		mktmakegubun[1];		/*  �����������񿩺�     */
//	char		expname[20];		/*  ǥ�������     */
//	char		bdate[8];		/*  ������������     */
//	char		xday[4];		/* BATCH���̰���ϼ�     */
//	char		bjandatecnt[4];		/* �����������ϼ�     */
//	char		chetime[10];		/*  ü��ð�     */
//	char		price[7];		/*  ���簡     */
//	char		open[7];		/*  �ð�     */
//	char		high[7];		/*  ��     */
//	char		low[7];		/*  ����     */
//	char		sign[1];		/*  ���ϴ�񱸺�     */
//	char		change[7];		/*  ���ϴ��     */
//	char		volume[7];		/*  �����ŷ���     */
//	char		value[11];		/*  �����ŷ����     */
//	char		cvolume[7];		/*  �Ǻ�ü�����     */
//	char		cgubun[1];		/*  ü�ᱸ��     */
//	char		mdvolume[8];		/* �ŵ������ŷ���     */
//	char		msvolume[8];		/*  �ż������ŷ���     */
//	char		mdchecnt[5];		/* �ŵ�����ü��Ǽ�     */
//	char		mschecnt[5];		/*  �ż�����ü��Ǽ�     */
//	char		mdvolume_h[7];		/*  �ŵ�ü�����     */
//	char		msvolume_h[7];		/*  �ż�ü�����     */
//	char		hotime[10];		/*  ���Žð�     */
//	char		offerho1[7];		/* �ŵ�ȣ��1     */
//	char		bidho1[7];		/*  �ż�ȣ��1     */
//	char		offerrem1[7];		/*  �ŵ�ȣ������1     */
//	char		bidrem1[7];		/*  �ż�ȣ������1     */
//	char		offercnt1[5];		/*  �ŵ�ȣ���Ǽ�1     */
//	char		bidcnt1[5];		/*  �ż�ȣ���Ǽ�1     */
//	char		offerremcha1[7];		/*  �����ŵ�������1     */
//	char		bidremcha1[7];		/*  �����ż�������1     */
//	char		offercntcha1[5];		/*  �����ŵ����Ǽ�1     */
//	char		bidcntcha1[5];		/*  �����ż����Ǽ�1     */
//	char		offerho2[7];		/*  �ŵ�ȣ��2     */
//	char		bidho2[7];		/*  �ż�ȣ��2     */
//	char		offerrem2[7];		/*  �ŵ�ȣ������2     */
//	char		bidrem2[7];		/*  �ż�ȣ������2     */
//	char		offercnt2[5];		/*  �ŵ�ȣ���Ǽ�2     */
//	char		bidcnt2[5];		/*  �ż�ȣ���Ǽ�2     */
//	char		offerremcha2[7];		/*  �����ŵ�������2     */
//	char		bidremcha2[7];		/*  �����ż�������2     */
//	char		offercntcha2[5];		/*  �����ŵ����Ǽ�2     */
//	char		bidcntcha2[5];		/*  �����ż����Ǽ�2     */
//	char		offerho3[7];		/*  �ŵ�ȣ��3     */
//	char		bidho3[7];		/*  �ż�ȣ��3     */
//	char		offerrem3[7];		/*  �ŵ�ȣ������3     */
//	char		bidrem3[7];		/*  �ż�ȣ������3     */
//	char		offercnt3[5];		/*  �ŵ�ȣ���Ǽ�3     */
//	char		bidcnt3[5];		/*  �ż�ȣ���Ǽ�3     */
//	char		offerremcha3[7];		/*  �����ŵ�������3     */
//	char		bidremcha3[7];		/*  �����ż�������3     */
//	char		offercntcha3[5];		/*  �����ŵ����Ǽ�3     */
//	char		bidcntcha3[5];		/*  �����ż����Ǽ�3     */
//	char		offerho4[7];		/*  �ŵ�ȣ��4     */
//	char		bidho4[7];		/*  �ż�ȣ��4     */
//	char		offerrem4[7];		/*  �ŵ�ȣ������4     */
//	char		bidrem4[7];		/*  �ż�ȣ������4     */
//	char		offercnt4[5];		/*  �ŵ�ȣ���Ǽ�4     */
//	char		bidcnt4[5];		/*  �ż�ȣ���Ǽ�4     */
//	char		offerremcha4[7];		/*  �����ŵ�������4     */
//	char		bidremcha4[7];		/*  �����ż�������4     */
//	char		offercntcha4[5];		/*  �����ŵ����Ǽ�4     */
//	char		bidcntcha4[5];		/*  �����ż����Ǽ�4     */
//	char		offerho5[7];		/*  �ŵ�ȣ��5     */
//	char		bidho5[7];		/*  �ż�ȣ��5     */
//	char		offerrem5[7];		/*  �ŵ�ȣ������5     */
//	char		bidrem5[7];		/*  �ż�ȣ������5     */
//	char		offercnt5[5];		/*  �ŵ�ȣ���Ǽ�5     */
//	char		bidcnt5[5];		/*  �ż�ȣ���Ǽ�5     */
//	char		offerremcha5[7];		/*  �����ŵ�������5     */
//	char		bidremcha5[7];		/*  �����ż�������5     */
//	char		offercntcha5[5];		/*  �����ŵ����Ǽ�5     */
//	char		bidcntcha5[5];		/*  �����ż����Ǽ�5     */
//	char		totofferrem[7];		/*  �ŵ�ȣ���Ѽ���     */
//	char		totbidrem[7];		/*  �ż�ȣ���Ѽ���     */
//	char		totoffercnt[6];		/*  �ŵ�ȣ���ѰǼ�     */
//	char		totbidcnt[6];		/* �ż�ȣ���ѰǼ�     */
//	char		totofferremcha[7];		/*  �����ŵ�����Ѽ���     */
//	char		totbidremcha[7];		/*  �����ż�����Ѽ���     */
//	char		totoffercntcha[6];		/* �����ŵ�����ѰǼ�     */
//	char		totbidcntcha[6];		/* �����ż�����ѰǼ�     */
//	char		jgubun[2];		/*  ���뱸��     */
//	char		yakdate[8];		/*  ü������     */
//	char		mggubun[1];		/*  �̰�����������     */
//	char		openyak[7];		/*  �̰�����������     */
//	char		openyakcha[7];		/* �̰������������������     */
//	char		openopenyak[7];		/*  �̰��������ð�     */
//	char		highopenyak[7];		/*  �̰���������     */
//	char		lowopenyak[7];		/*  �̰�����������     */
//	char		marginprice[7];		/*  �Ÿ����űݱ��ذ�     */
//	char		margingubun[1];		/*  �Ÿ����űݱ��ذ�����     */
//	char		lastprice[16];		/*  �Ǹ�����������     */
//	char		lastpricegubun[1];		/*  �Ǹ����������ݱ���     */
//	char		baseprice[7];		/*  �������������     */
//	char		eqva[6];		/*  KOSPI�     */
//	char		xtime[8];		/*  ��ǥ����ð�     */
//	char		theoryprice[13];		/*  �����̷а�     */
//	char		delt[6];		/*  ��Ÿ     */
//	char		gama[6];		/*  ����     */
//	char		ceta[6];		/*  ��Ÿ     */
//	char		vega[6];		/*  ����     */
//	char		rhox[6];		/*  �ο�     */
//	char		impv[6];		/*  ���纯����     */
//	char		timevalue[6];		/*  �ð���ġ     */
//	char		offerimpv[6];		/*  �ŵ�ȣ�����纯����     */
//	char		bidimpv[6];		/*  �ŵ�ȣ�����纯����     */
//	char		histimpv[6];		/*  ������������     */
//	char		b_theoryprice[6];		/*  �����̷а����׸�     */
//	char		b_delt[6];		/*  ��Ÿ���׸�     */
//	char		b_gama[6];		/*  �������׸�     */
//	char		b_ceta[6];		/*  ��Ÿ���׸�     */
//	char		b_vega[6];		/*  �������׸�     */
//	char		b_rhox[6];		/*  �ο����׸�     */
//	char		drate[6];		/*  �����     */
//	char		b_volrate[6];		/*  ���ϰŷ�����     */
//}EU_f3201_OutRec1;

typedef struct _EU_f3201_OutRec1 {
	char		optcode[8];		/*  �����ڵ�     */
	char		recprice[7];		/*  ���ذ���     */
	char		hprice[7];		/*  ���Ѱ�     */
	char		lprice[7];		/*  ���Ѱ�     */
	char		price[7];		/*  ���簡     */
	char		offerho1[8];		/*  �ŵ�ȣ��1     */
	char		offerho2[8];		/*  �ŵ�ȣ��2     */
	char		offerho3[8];		/*  �ŵ�ȣ��3     */
	char		bidho1[8];		/*  �ż�ȣ��1     */
	char		bidho2[8];		/*  �ż�ȣ��2     */
	char		bidho3[8];		/*  �ż�ȣ��3     */
} EU_f3201_OutRec1;

/*************************************************************************************/
class CEU_f3201
{
public: //Variable
	CString		m_szoptcode;		/*  �����ڵ�     */
	CString		m_szrecprice;		/*  ���ذ���     */
	CString		m_szhprice;		/*  ���Ѱ�     */
	CString		m_szlprice;		/*  ���Ѱ�     */
	CString		m_szprice;		/*  ���簡     */
	CString		m_szofferho1;		/*  �ŵ�ȣ��1     */
	CString		m_szofferho2;		/*  �ŵ�ȣ��2     */
	CString		m_szofferho3;		/*  �ŵ�ȣ��3     */
	CString		m_szbidho1;		/*  �ż�ȣ��1     */
	CString		m_szbidho2;		/*  �ż�ȣ��2     */
	CString		m_szbidho3;		/*  �ż�ȣ��3     */

public:
	CEU_f3201()
	{
	}

	virtual void  Convert(EU_f3201_OutRec1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, optcode);		/*  �����ڵ�     */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/*  ���ذ���     */
		DATA_MEMCOPY(sTmp, pInput, hprice);		/*  ���Ѱ�     */
		DATA_MEMCOPY(sTmp, pInput, lprice);		/*  ���Ѱ�     */
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

