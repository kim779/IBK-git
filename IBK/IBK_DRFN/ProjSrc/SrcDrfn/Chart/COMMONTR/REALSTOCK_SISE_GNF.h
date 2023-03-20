#include "Data_Common.h"

//--------------------------------------------------------------------
//     �ֽ�
//--------------------------------------------------------------------
typedef struct 
{
	char		dgubun[4];		/*  [Data ����: CAS1]     */
	char		jmcode[10];		/*  [�����ڵ�]     */
	char		chetime[6];		/*  [ü��ð� ]     */
	char		open[10];		/*  [�ð�]     */
	char		high[10];		/*  [��]     */
	char		low[10];		/*  [����]     */
	char		price[10];		/*  [ü�ᰡ]     */
	char		sign[1];		/*  [���ϴ�񱸺�]     */
	char		change[10];		/*  [���ϴ��]     */
	char		drate[6];		/*  [���ϴ�� �����]     */
	char		presign[1];		/*  [������񱸺�]     */
	char		cvolume[6];		/*  [ü�ᷮ]     */
	char		cgubun[1];		/*  [ü�ᱸ�� ]     */
	char		volume[10];		/*  [�����ŷ���]     */
	char		value[12];		/*  [�����ŷ����]     */
	char		ssvolume[10];	/*  [��°ŷ���]     */
	char		hrvolume[10];	/*  [�϶��ŷ���]     */
	char		sscnt[6];		/*  [��°Ǽ�]     */
	char		hrcnt[6];		/*  [�϶��Ǽ�]     */
	char		offerho1[10];	/*  [�ŵ�ȣ��]     */
	char		bidho1[10];		/*  [�ż�ȣ��]     */
	char		offerrem1[8];	/*  [�ŵ��ܷ�]     */
	char		bidrem1[8];		/*  [�ż��ܷ�]     */
	char		seqno[8];		/*  [�ŷ���ȣ]     */
	char		price_d[15];	/*  [���� ��ȯ�� ü�ᰡ]     */
	char		bday[8];		/*  [������ ]     */
	char		kjmcode[10];	/*  [�����ڵ�]     */
	char		chetime_g[6];	/*  [���� ü��ð� ]     */
} STREAL_GNF_STOCK_SISE;



class CREAL_GNF_STOCK_SISE
{
public: //Variable
	CString m_szdgubun;		/*  [Data ����: CAS1]     */
	CString m_szjmcode;		/*  [�����ڵ�]     */
	CString m_szchetime;		/*  [ü��ð� ]     */
	CString m_szopen;		/*  [�ð�]     */
	CString m_szhigh;		/*  [��]     */
	CString m_szlow;		/*  [����]     */
	CString m_szprice;		/*  [ü�ᰡ]     */
	CString m_szsign;		/*  [���ϴ�񱸺�]     */
	CString m_szchange;		/*  [���ϴ��]     */
	CString m_szdrate;		/*  [���ϴ�� �����]     */
	CString m_szpresign;		/*  [������񱸺�]     */
	CString m_szcvolume;		/*  [ü�ᷮ]     */
	CString m_szcgubun;		/*  [ü�ᱸ�� ]     */
	CString m_szvolume;		/*  [�����ŷ���]     */
	CString m_szvalue;		/*  [�����ŷ����]     */
	CString m_szssvolume;	/*  [��°ŷ���]     */
	CString m_szhrvolume;	/*  [�϶��ŷ���]     */
	CString m_szsscnt;		/*  [��°Ǽ�]     */
	CString m_szhrcnt;		/*  [�϶��Ǽ�]     */
	CString m_szofferho1;	/*  [�ŵ�ȣ��]     */
	CString m_szbidho1;		/*  [�ż�ȣ��]     */
	CString m_szofferrem1;	/*  [�ŵ��ܷ�]     */
	CString m_szbidrem1;		/*  [�ż��ܷ�]     */
	CString m_szseqno;		/*  [�ŷ���ȣ]     */
	CString m_szprice_d;	/*  [���� ��ȯ�� ü�ᰡ]     */
	CString m_szbday;		/*  [������ ]     */
	CString m_szkjmcode;	/*  [�����ڵ�]     */
	CString m_szchetime_g;	/*  [���� ü��ð� ]     */

public:
	CREAL_GNF_STOCK_SISE()
	{
	}

	virtual void  Convert(STREAL_GNF_STOCK_SISE* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);	
		DATA_MEMCOPY(sTmp, pInput, jmcode);	
		DATA_MEMCOPY(sTmp, pInput, chetime);	
		DATA_MEMCOPY(sTmp, pInput, open);	
		DATA_MEMCOPY(sTmp, pInput, high);	
		DATA_MEMCOPY(sTmp, pInput, low);	
		DATA_MEMCOPY(sTmp, pInput, price);	
		DATA_MEMCOPY(sTmp, pInput, sign);	
		DATA_MEMCOPY(sTmp, pInput, change);	
		DATA_MEMCOPY(sTmp, pInput, drate);	
		DATA_MEMCOPY(sTmp, pInput, presign);	
		DATA_MEMCOPY(sTmp, pInput, cvolume);	
		DATA_MEMCOPY(sTmp, pInput, cgubun);	
		DATA_MEMCOPY(sTmp, pInput, volume);	
		DATA_MEMCOPY(sTmp, pInput, value);	
		DATA_MEMCOPY(sTmp, pInput, ssvolume);
		DATA_MEMCOPY(sTmp, pInput, hrvolume);
		DATA_MEMCOPY(sTmp, pInput, sscnt);	
		DATA_MEMCOPY(sTmp, pInput, hrcnt);	
		DATA_MEMCOPY(sTmp, pInput, offerho1);
		DATA_MEMCOPY(sTmp, pInput, bidho1);	
		DATA_MEMCOPY(sTmp, pInput, offerrem1);
		DATA_MEMCOPY(sTmp, pInput, bidrem1);	
		DATA_MEMCOPY(sTmp, pInput, seqno);	
		DATA_MEMCOPY(sTmp, pInput, price_d);
		DATA_MEMCOPY(sTmp, pInput, bday);	
		DATA_MEMCOPY(sTmp, pInput, kjmcode);
		DATA_MEMCOPY(sTmp, pInput, chetime_g);
	}
};


