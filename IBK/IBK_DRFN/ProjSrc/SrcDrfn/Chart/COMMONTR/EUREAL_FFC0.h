#if !defined(AFX_EUREAL_FFC0__H_304364EA_27F5_429e_85A4_EB5A23698923_INCLUDED_)
#define AFX_EUREAL_FFC0__H_304364EA_27F5_429e_85A4_EB5A23698923_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     ����
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [Data ����]				*/
	char		kfutcode[8];		/*  [���������ڵ�]			*/
	char		chetime[6];			/*  [ü��ð�]				*/
	char		sign[1];			/*  [2:��� 3:���� 5:�϶�]  */
	char		change[6];			/*  [���ϴ��]				*/
	char		drate[6];			/*  [�����]				*/
	char		price[6];			/*  [���簡]				*/
	char		open[6];			/*  [�ð�]					*/
	char		high[6];			/*  [��]					*/
	char		low[6];				/*  [����]					*/
	char		cgubun[1];			/*  [ü�ᱸ��]				*/
	char		cvolume[6];			/*  [ü�ᷮ]				*/
	char		volume[12];			/*  [�����ŷ���]			*/
	char		volchange[12];		/*  [�ŷ������ϴ��]		*/
	char		voldrate[8];		/*  [�ŷ��������]			*/
	char		value[12];			/*  [�����ŷ����]			*/
	char		mdvolume[12];		/*  [�ŵ����� ü�ᷮ]		*/
	char		mdchecnt[8];		/*  [�ŵ����� ü��Ǽ�]     */
	char		msvolume[12];		/*  [�ż����� ü�ᷮ]		*/
	char		mschecnt[8];		/*  [�ż����� ü��Ǽ�]     */
	char		cpower[9];			/*  [ü�ᰭ��]				*/
	char		offerho1[6];		/*  [�ŵ�ȣ��1]				*/
	char		bidho1[6];			/*  [�ż�ȣ��1]				*/
	char		openyak[8];			/*  [�̰�����������]		*/
	char		openyakchange[8];	/*  [�̰����������ϴ��]	*/
	char		k200jisu[6];		/*  [KOSPI200 ����]			*/
	char		theoryprice[6];		/*  [�̷а�]				*/
	char		grate[6];			/*  [������]				*/
	char		kasis[6];			/*  [������]				*/
	char		sbasis[6];			/*  [���� BASIS]			*/
	char		ibasis[6];			/*  [�̷� BASIS]			*/
	char		jgubun[2];			/*  [�� ����]			*/
	char		jnilvolume[12];		/*  [���� ���ð��� �ŷ���]  */
	char		futcode[8];			/*  [�����ڵ�]				*/
} stEUREAL_FFC0;	//65

class CEUREAL_FFC0
{
public: //Variable
	CString m_szdgubun;			/*  [Data ����]				*/
	CString m_szkfutcode;		/*  [���������ڵ�]			*/
	CString m_szchetime;		/*  [ü��ð�]				*/
	CString m_szsign;			/*  [2:��� 3:���� 5:�϶�]  */
	CString m_szchange;			/*  [���ϴ��]				*/
	CString m_szdrate;			/*  [�����]				*/
	CString m_szprice;			/*  [���簡]				*/
	CString m_szopen;			/*  [�ð�]					*/
	CString m_szhigh;			/*  [��]					*/
	CString m_szlow;			/*  [����]					*/
	CString m_szcgubun;			/*  [ü�ᱸ��]				*/
	CString m_szcvolume;		/*  [ü�ᷮ]				*/
	CString m_szvolume;			/*  [�����ŷ���]			*/
	CString m_szvolchange;		/*  [�ŷ������ϴ��]		*/
	CString m_szvoldrate;		/*  [�ŷ��������]			*/
	CString m_szvalue;			/*  [�����ŷ����]			*/
	CString m_szmdvolume;		/*  [�ŵ����� ü�ᷮ]		*/
	CString m_szmdchecnt;		/*  [�ŵ����� ü��Ǽ�]     */
	CString m_szmsvolume;		/*  [�ż����� ü�ᷮ]		*/
	CString m_szmschecnt;		/*  [�ż����� ü��Ǽ�]     */
	CString m_szcpower;			/*  [ü�ᰭ��]				*/
	CString m_szofferho1;		/*  [�ŵ�ȣ��1]				*/
	CString m_szbidho1;			/*  [�ż�ȣ��1]				*/
	CString m_szopenyak;		/*  [�̰�����������]		*/
	CString m_szopenyakchange;	/*  [�̰����������ϴ��]	*/
	CString m_szk200jisu;		/*  [KOSPI200 ����]			*/
	CString m_sztheoryprice;	/*  [�̷а�]				*/
	CString m_szgrate;			/*  [������]				*/
	CString m_szkasis;			/*  [������]				*/
	CString m_szsbasis;			/*  [���� BASIS]			*/
	CString m_szibasis;			/*  [�̷� BASIS]			*/
	CString m_szjgubun;			/*  [�� ����]			*/
	CString m_szjnilvolume;		/*  [���� ���ð��� �ŷ���]  */
	CString m_szfutcode;		/*  [�����ڵ�]				*/

public:
	CEUREAL_FFC0()
	{
	}

	virtual void  Convert(stEUREAL_FFC0* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data ����]				*/
		DATA_MEMCOPY(sTmp, pInput, kfutcode);		/*  [���������ڵ�]			*/
		DATA_MEMCOPY(sTmp, pInput, chetime);		/*  [ü��ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, sign);			/*  [2:��� 3:���� 5:�϶�]  */
		DATA_MEMCOPY(sTmp, pInput, change);			/*  [���ϴ��]				*/
		DATA_MEMCOPY(sTmp, pInput, drate);			/*  [�����]				*/
		DATA_MEMCOPY(sTmp, pInput, price);			/*  [���簡]				*/
		DATA_MEMCOPY(sTmp, pInput, open);			/*  [�ð�]					*/
		DATA_MEMCOPY(sTmp, pInput, high);			/*  [��]					*/
		DATA_MEMCOPY(sTmp, pInput, low);			/*  [����]					*/
		DATA_MEMCOPY(sTmp, pInput, cgubun);			/*  [ü�ᱸ��]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);		/*  [ü�ᷮ]				*/
		DATA_MEMCOPY(sTmp, pInput, volume);			/*  [�����ŷ���]			*/
		DATA_MEMCOPY(sTmp, pInput, volchange);		/*  [�ŷ������ϴ��]		*/
		DATA_MEMCOPY(sTmp, pInput, voldrate);		/*  [�ŷ��������]			*/
		DATA_MEMCOPY(sTmp, pInput, value);			/*  [�����ŷ����]			*/
		DATA_MEMCOPY(sTmp, pInput, mdvolume);		/*  [�ŵ����� ü�ᷮ]		*/
		DATA_MEMCOPY(sTmp, pInput, mdchecnt);		/*  [�ŵ����� ü��Ǽ�]     */
		DATA_MEMCOPY(sTmp, pInput, msvolume);		/*  [�ż����� ü�ᷮ]		*/
		DATA_MEMCOPY(sTmp, pInput, mschecnt);		/*  [�ż����� ü��Ǽ�]     */
		DATA_MEMCOPY(sTmp, pInput, cpower);			/*  [ü�ᰭ��]				*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [�ŵ�ȣ��1]				*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [�ż�ȣ��1]				*/
		DATA_MEMCOPY(sTmp, pInput, openyak);		/*  [�̰�����������]		*/
		DATA_MEMCOPY(sTmp, pInput, openyakchange);	/*  [�̰����������ϴ��]	*/
		DATA_MEMCOPY(sTmp, pInput, k200jisu);		/*  [KOSPI200 ����]			*/
		DATA_MEMCOPY(sTmp, pInput, theoryprice);	/*  [�̷а�]				*/
		DATA_MEMCOPY(sTmp, pInput, grate);			/*  [������]				*/
		DATA_MEMCOPY(sTmp, pInput, kasis);			/*  [������]				*/
		DATA_MEMCOPY(sTmp, pInput, sbasis);			/*  [���� BASIS]			*/
		DATA_MEMCOPY(sTmp, pInput, ibasis);			/*  [�̷� BASIS]			*/
		DATA_MEMCOPY(sTmp, pInput, jgubun);			/*  [�� ����]			*/
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/*  [���� ���ð��� �ŷ���]  */
		DATA_MEMCOPY(sTmp, pInput, futcode);		/*  [�����ڵ�]				*/
	}
};

#endif // AFX_EUREAL_FFC0__H_304364EA_27F5_429e_85A4_EB5A23698923_INCLUDED_
