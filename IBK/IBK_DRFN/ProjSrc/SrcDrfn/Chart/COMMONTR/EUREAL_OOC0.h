#if !defined(AFX_EUREAL_OOC0__H_497DAEEA_D24D_4317_A27B_08E51CA94FFF_INCLUDED_)
#define AFX_EUREAL_OOC0__H_497DAEEA_D24D_4317_A27B_08E51CA94FFF_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     �ɼ�
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [Data ����]				*/
	char		koptcode[8];		/*  [�ɼǴ����ڵ�]			*/
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
	char		openyakchange[8];	/*  [�̰����������ϴ��]    */
	char		k200jisu[6];		/*  [KOSPI200 ����]			*/
	char		eqva[7];			/*  [KOSPI �]			*/
	char		theoryprice[6];		/*  [�̷а�]				*/
	char		grate[6];			/*  [������]				*/
	char		kasis[6];			/*  [������]				*/
	char		impv[6];			/*  [���纯����]			*/
	char		intvalue[6];		/*  [���簡ġ]				*/
	char		timevalue[6];		/*  [�ð���ġ]				*/
	char		jgubun[2];			/*  [�� ����]			*/
	char		jnilvolume[12];		/*  [���� ���ð��� �ŷ���]  */
	char		optcode[8];			/*  [�ɼǴ����ڵ�]			*/
} stEUREAL_OOC0;	//66

class CEUREAL_OOC0
{
public: //Variable
	CString m_szdgubun;			/*  [Data ����]				*/
	CString m_szkoptcode;		/*  [�ɼǴ����ڵ�]			*/
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
	CString m_szopenyakchange;	/*  [�̰����������ϴ��]    */
	CString m_szk200jisu;		/*  [KOSPI200 ����]			*/
	CString m_szeqva;			/*  [KOSPI �]			*/
	CString m_sztheoryprice;	/*  [�̷а�]				*/
	CString m_szgrate;			/*  [������]				*/
	CString m_szkasis;			/*  [������]				*/
	CString m_szimpv;			/*  [���纯����]			*/
	CString m_szintvalue;		/*  [���簡ġ]				*/
	CString m_sztimevalue;		/*  [�ð���ġ]				*/
	CString m_szjgubun;			/*  [�� ����]			*/
	CString m_szjnilvolume;		/*  [���� ���ð��� �ŷ���]  */
	CString m_szoptcode;		/*  [�ɼǴ����ڵ�]			*/

public:
	CEUREAL_OOC0()
	{
	}

	virtual void  Convert(stEUREAL_OOC0* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data ����]				*/
		DATA_MEMCOPY(sTmp, pInput, koptcode);		/*  [�ɼǴ����ڵ�]			*/
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
		DATA_MEMCOPY(sTmp, pInput, openyakchange);	/*  [�̰����������ϴ��]    */
		DATA_MEMCOPY(sTmp, pInput, k200jisu);		/*  [KOSPI200 ����]			*/
		DATA_MEMCOPY(sTmp, pInput, eqva);			/*  [KOSPI �]			*/
		DATA_MEMCOPY(sTmp, pInput, theoryprice);	/*  [�̷а�]				*/
		DATA_MEMCOPY(sTmp, pInput, grate);			/*  [������]				*/
		DATA_MEMCOPY(sTmp, pInput, kasis);			/*  [������]				*/
		DATA_MEMCOPY(sTmp, pInput, impv);			/*  [���纯����]			*/
		DATA_MEMCOPY(sTmp, pInput, intvalue);		/*  [���簡ġ]				*/
		DATA_MEMCOPY(sTmp, pInput, timevalue);		/*  [�ð���ġ]				*/
		DATA_MEMCOPY(sTmp, pInput, jgubun);			/*  [�� ����]			*/
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/*  [���� ���ð��� �ŷ���]  */
		DATA_MEMCOPY(sTmp, pInput, optcode);		/*  [�ɼǴ����ڵ�]			*/
	}
};

#endif // AFX_EUREAL_OOC0__H_497DAEEA_D24D_4317_A27B_08E51CA94FFF_INCLUDED_
