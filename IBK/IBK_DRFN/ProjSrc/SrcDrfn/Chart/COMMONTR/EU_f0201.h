/*************************************************************************************/
/*  1. ���α׷�ID : EU_f0201                                                         */
/*  2. ���α׷��� : �����ɼ�(ȣ��)                                                   */
/*  3. ���񽺸�   : EU_f0201                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_f0201_H__
#define __EU_TRAN_EU_f0201_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define EU_TRAN                       "EU_f0201"
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
}EU_f0201_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char EU_f0201_OutRec1_count[4];
//typedef struct
//{
//	char		futcode[8];		/*  �����ڵ�     */
//	char		expcode[12];		/*  �����ڵ�     */
//	char		expname[20];		/*  ǥ�������     */
//	char		hname[30];		/*  �ѱ������     */
//	char		ename[30];		/*  ���������     */
//	char		sname[15];		/*  ��������     */
//	char		name[4];		/*  �����     */
//	char		listdate[8];		/*  ������     */
//	char		listdatecnt[3];		/*  �����ϼ�     */
//	char		jandatecnt[3];		/*  �����ϼ�     */
//	char		lastdate[8];		/*  �����ŷ���     */
//	char		lastdategubun[1];		/*  �����ŷ��ϱ���     */
//	char		divideratio[9];		/*  ���������̷���ġ     */
//	char		recpricegubun[1];		/*  ���ذ��ݱ���     */
//	char		recprice[6];		/*  ���ذ���     */
//	char		theorypricek[6];		/*  �̷а�     */
//	char		theorypriceg[6];		/*  �̷а�     */
//	char		cdratio[6];		/*  CD�ݸ�     */
//	char		uplmtprice[6];		/*  ���Ѱ�     */
//	char		dnlmtprice[6];		/*  ���Ѱ�     */
//	char		jnilsetgubun[1];		/*  �������갡����     */
//	char		jnilsetprice[6];		/*  �������갡     */
//	char		jnilclosegubun[1];		/*  ������������     */
//	char		jnilclose[6];		/*  ��������     */
//	char		jnilvolume[8];		/*  ����ü�����     */
//	char		jnilgmvolume[8];		/*  ���ϱٿ���ü�����     */
//	char		jnilwmvolume[8];		/*  ���Ͽ�����ü�����     */
//	char		jnilvalue[15];		/*  ����ü����     */
//	char		jnilgmvalue[15];		/*  ���ϱٿ����ŷ����     */
//	char		jnilwmvalue[15];		/*  ���Ͽ������ŷ����     */
//	char		jnilopenyak[8];		/*  ���Ϲ̰�������������     */
//	char		listhprice[6];		/*  �������ְ�(SIGN����)     */
//	char		listhdate[8];		/*  �������ְ���     */
//	char		listhrate[6];		/*  �������ְ�����     */
//	char		listlprice[6];		/*  ������������(SIGN����)     */
//	char		listldate[8];		/*  ��������������     */
//	char		listlrate[6];		/*  ����������������     */
//	char		ipwhedate[8];		/*  ��ȸ����     */
//	char		mrkgubun[1];		/*  ���尡��뱸��     */
//	char		jogengubun[1];		/*  ���Ǻ���������뱸��     */
//	char		choiygubun[1];		/*  ��������������뱸��     */
//	char		lastprice[6];		/*  ����������     */
//	char		lastpricegubun[1];		/*  ��������������     */
//	char		cbhprice[6];		/*  CB������Ѱ�     */
//	char		cblprice[6];		/*  CB�������Ѱ�     */
//	char		jnilmmchetime[8];		/*  ���������Ÿ�ü��ð�     */
//	char		jnilspgmclose[6];		/*  ���Ͻ�������ٿ�������     */
//	char		jnilspwmclose[6];		/*  ���Ͻ����������������     */
//	char		gexpcode[12];		/*  ��������ٿ���ǥ���ڵ�     */
//	char		wexpcode[12];		/*  �������������ǥ���ڵ�     */
//	char		tradeunit[16];		/*  �ŷ�����     */
//	char		chetime[6];		/*  ü��ð�     */
//	char		price[6];		/*  ���簡     */
//	char		open[6];		/*  �ð�     */
//	char		high[6];		/*  ��     */
//	char		low[6];		/*  ����     */
//	char		sign[1];		/*  ���ϴ�񱸺�     */
//	char		change[6];		/*  ���ϴ��     */
//	char		volume[7];		/*  �����ŷ���     */
//	char		value[15];		/*  �����ŷ����     */
//	char		vol_change[6];		/*  ���ϰŷ������     */
//	char		diff_rate[6];		/*  �����     */
//	char		cvolume[6];		/*  �Ǻ�ü�����     */
//	char		cgubun[1];		/*  ü�ᱸ��     */
//	char		mdvolume[7];		/*  �ŵ������ŷ���     */
//	char		msvolume[7];		/*  �ż������ŷ���     */
//	char		mdchecnt[6];		/*  �ŵ�����ü��Ǽ�     */
//	char		mschecnt[6];		/* �ż�����ü��Ǽ�     */
//	char		gmprice[6];		/*  �ٿ���������������     */
//	char		wmprice[6];		/*  ������������������     */
//	char		gmvolume[7];		/*  �ٿ���ü�����     */
//	char		wmvolume[7];		/*  ������ü�����     */
//	char		gmvalue[15];		/*  �ٿ����ŷ����     */
//	char		wmvalue[15];		/*  �������ŷ����     */
//	char		mdvolume_h[7];		/*  �ŵ�ü�����(ȣ������)     */
//	char		msvolume_h[7];		/*  �ż�ü�����(ȣ������)     */
//	char		hotime[6];		/*  ȣ�����Žð�     */
//	char		offerho1[6];		/*  �ŵ�ȣ��1     */
//	char		bidho1[6];		/*  �ż�ȣ��1     */
//	char		offerrem1[6];		/*  �ŵ�ȣ������1     */
//	char		bidrem1[6];		/*  �ż�ȣ������1     */
//	char		offercnt1[4];		/*  �ŵ�ȣ���Ǽ�1     */
//	char		bidcnt1[4];		/*  �ż�ȣ���Ǽ�1     */
//	char		offerho2[6];		/*  �ŵ�ȣ��2     */
//	char		bidho2[6];		/*  �ż�ȣ��2     */
//	char		offerrem2[6];		/*  �ŵ�ȣ������2     */
//	char		bidrem2[6];		/*  �ż�ȣ������2     */
//	char		offercnt2[4];		/*  �ŵ�ȣ���Ǽ�2     */
//	char		bidcnt2[4];		/*  �ż�ȣ���Ǽ�2     */
//	char		offerho3[6];		/*  �ŵ�ȣ��3     */
//	char		bidho3[6];		/*  �ż�ȣ��3     */
//	char		offerrem3[6];		/*  �ŵ�ȣ������3     */
//	char		bidrem3[6];		/*  �ż�ȣ������3     */
//	char		offercnt3[4];		/*  �ŵ�ȣ���Ǽ�3     */
//	char		bidcnt3[4];		/*  �ż�ȣ���Ǽ�3     */
//	char		offerho4[6];		/*  �ŵ�ȣ��4     */
//	char		bidho4[6];		/*  �ż�ȣ��4     */
//	char		offerrem4[6];		/*  �ŵ�ȣ������4     */
//	char		bidrem4[6];		/*  �ż�ȣ������4     */
//	char		offercnt4[4];		/*  �ŵ�ȣ���Ǽ�4     */
//	char		bidcnt4[4];		/*  �ż�ȣ���Ǽ�4     */
//	char		offerho5[6];		/*  �ŵ�ȣ��5     */
//	char		bidho5[6];		/*  �ż�ȣ��5     */
//	char		offerrem5[6];		/*  �ŵ�ȣ������5     */
//	char		bidrem5[6];		/*  �ż�ȣ������5     */
//	char		offercnt5[4];		/*  �ŵ�ȣ���Ǽ�5     */
//	char		bidcnt5[4];		/*  �ż�ȣ���Ǽ�5     */
//	char		totofferrem[6];		/*  �ŵ�ȣ���Ѽ���     */
//	char		totbidrem[6];		/*  �ż�ȣ���Ѽ���     */
//	char		totoffercnt[4];		/*  �ŵ�ȣ���ѰǼ�     */
//	char		totbidcnt[4];		/*  �ż�ȣ���ѰǼ�     */
//	char		jgubun[2];		/*  ���뱸��     */
//	char		yakdate[8];		/*  ü������     */
//	char		mggubun[1];		/*  �̰�����������     */
//	char		jsprice[6];		/*  ���갡��     */
//	char		jsgubun[1];		/*  ���갡����     */
//	char		k200jisu[8];		/*  KOSPI200����     */
//	char		theoryprice[6];		/*  �̷а�     */
//	char		kasis[6];		/*  ������     */
//	char		sbasis[6];		/*  ����BASIS     */
//	char		ibasis[6];		/* �̷�BASIS     */
//	char		openyak[8];		/*  �̰�����������     */
//	char		openopenyak[8];		/*  �̰��������ð�     */
//	char		highopenyak[8];		/* �̰���������     */
//	char		lowopenyak[8];		/*  �̰�����������     */
//	char		openupdn[8];		/*  �̰�����������     */
//	char		offer2[6];		/*  2������     */
//	char		offer1[6];		/*  1������     */
//	char		pivot[6];		/*  �Ǻ���     */
//	char		supp1[6];		/*  1������     */
//	char		supp2[6];		/*  2������     */
//	char		seqno[4];		/*  ����SEQ��ȣ     */
//	char		opkind[1];		/*  �ɼ�����     */
//	char		eitem[2];		/*  �ŷ��������     */
//	char		lastmonth[6];		/*  ������     */
//	char		actprice[6];		/*  ��簡��     */
//	char		queryno[4];		/*  ��ȸ����     */
//	char		actgubun[3];		/*  �Ǹ������������     */
//	char		lastgubun[1];		/* �ֱٿ�������     */
//	char		atmgubun[1];		/*  ATM����     */
//	char		startdate[8];		/* ���ʰŷ���     */
//	char		newgubun[1];		/*  �űԱ���     */
//	char		newdate[8];		/*  �ű�����     */
//	char		jnilopen[6];		/*  ���Ͻð�     */
//	char		jnilhigh[6];		/*  ���ϰ�     */
//	char		jnillow[6];		/*  ��������     */
//	char		bmarginprice[6];		/*  �Ÿ����űݱ��ذ�     */
//	char		bmargingubun[1];		/*  �Ÿ����űݱ��ذ�����     */
//	char		hprice[6];		/* ����ȣ������(����ġ)     */
//	char		lprice[6];		/*  ����ȣ������(����ġ)     */
//	char		basevalue[9];		/*  ���ݴ������ذ�(3.00)     */
//	char		upunit[3];		/*  ȣ������(0.01)     */
//	char		dwunit[3];		/*  ȣ������(0.05)     */
//	char		yearhdate[8];		/*  �����ְ���     */
//	char		yearhprice[6];		/*  �����ְ�     */
//	char		yearldate[8];		/*  ������������     */
//	char		yearlprice[6];		/* ����������     */
//	char		bimpv[6];		/*  ���ϳ��纯����     */
//	char		marginprice[8];		/*  �Ÿ����űݱ��ذ�     */
//	char		margingubun[1];		/*  �Ÿ����űݱ��ذ�����     */
//	char		eqva[8];		/*  KOSPI�     */
//	char		xtime[8];		/*  ��ǥ����ð�     */
//	char		delt[7];		/*  ��Ÿ     */
//	char		gama[7];		/*  ����     */
//	char		ceta[12];		/*  ��Ÿ     */
//	char		vega[12];		/*  ����     */
//	char		rhox[12];		/*  �ο�     */
//	char		impv[9];		/*  ���纯����     */
//	char		imvalue[9];		/*  ���簡ġ     */
//	char		timevalue[1];		/*  �ð���ġ     */
//	char		offerimpv[9];		/*  �ŵ�ȣ�����纯����     */
//	char		bidimpv[9];		/*  �ŵ�ȣ�����纯����     */
//	char		histimpv[9];		/*  ������������     */
//	char		b_theoryprice[14];		/*  �����̷а����׸�     */
//	char		b_delt[7];		/*  ��Ÿ���׸�     */
//	char		b_gama[7];		/*  �������׸�     */
//	char		b_ceta[7];		/*  ��Ÿ���׸�     */
//	char		b_vega[7];		/*  �������׸�     */
//	char		b_rhox[7];		/*  �ο����׸�     */
//	char		mimpvc1[9];		/*  �ŷ���Cal���س��纯����     */
//	char		mimpvc2[9];		/*  �ŷ����Cal���س��纯����     */
//	char		mimpvc3[9];		/*  �ŷ���Put���س��纯����     */
//	char		mimpvc4[9];		/*  �ŷ����Put���س��纯����     */
//	char		impvchk[9];		/*  �������ð����帶����Update�ߴ���     */
//	char		mktmakegubun[1];		/*  �����������񿩺�     */
//	char		xday[3];		/*  BATCH���̰�����ϼ�     */
//	char		b_bidrem1[6];		/*  ���ż��ܷ�1     */
//	char		b_bidrem2[6];		/*  ���ż��ܷ�2     */
//	char		b_bidrem3[6];		/*  ���ż��ܷ�3     */
//	char		b_bidrem4[6];		/*  ���ż��ܷ�4     */
//	char		b_bidrem5[6];		/*  ���ż��ܷ�5     */
//	char		tot_bidrem[8];		/*  �Ѽ��ż��ܷ�     */
//	char		ho_drate1[6];		/*  �ŵ�ȣ�������1     */
//	char		ho_drate2[6];		/*  �ŵ�ȣ�������2     */
//	char		ho_drate3[6];		/*  �ŵ�ȣ�������3     */
//	char		ho_drate4[6];		/*  �ŵ�ȣ�������4     */
//	char		ho_drate5[6];		/*  �ŵ�ȣ�������5     */
//	char		hb_drate1[6];		/*  �ż�ȣ�������1     */
//	char		hb_drate2[6];		/*  �ż�ȣ�������2     */
//	char		hb_drate3[6];		/*  �ż�ȣ�������3     */
//	char		hb_drate4[6];		/*  �ż�ȣ�������4     */
//	char		hb_drate5[6];		/*  �ż�ȣ�������5     */
//	char		kasis_d[6];		/*  ������     */
//	char		bjandatecnt[3];		/*  �����������ϼ�     */
//	char		totofferremcha[8];		/*  �����ŵ�����Ѽ���     */
//	char		totbidremcha[8];		/*  �����ż�����Ѽ���     */
//	char		totoffercntcha[8];		/*  �����ŵ�����ѰǼ�     */
//	char		totbidcntcha[8];		/*  �����ż�����ѰǼ�     */
//	char		yeuplmt[6];		/*  ���Ͽ�����Ѱ�     */
//	char		yednlmt[6];		/*  ���Ͽ������Ѱ�     */
//	char		yedate[8];		/* ��������ֹ�����     */
//}EU_f0201_OutRec1;

typedef struct _EU_f0201_OutRec1 {
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
} EU_f0201_OutRec1;

/*************************************************************************************/
class CEU_f0201
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
	CEU_f0201()
	{
	}

	virtual void  Convert(EU_f0201_OutRec1* pInput)
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

