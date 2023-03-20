#if !defined(AFX_EUREAL_KXH3__H_C61A6AD4_A32A_4238_8D21_0E7DE88C9537_INCLUDED_)
#define AFX_EUREAL_KXH3__H_C61A6AD4_A32A_4238_8D21_0E7DE88C9537_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     �ֽ�
//--------------------------------------------------------------------

typedef struct {
	char dgubun[4];			/*  [Data ����:]			*/
	char kshcode[6];		/*  [�����ڵ�]				*/
	char chetime[6];		/*  [ü��ð�]				*/
	char sign[1];			/*  [1:����]				*/
	char change[8];			/*  [���ϴ��,   ��]		*/
	char drate[6];			/*  [�����]				*/
	char price[8];			/*  [���簡,     ��]		*/
	char opentime[6];		/*  [�ð��ð�]				*/
	char open[8];			/*  [�ð�,       ��]		*/
	char hightime[6];		/*  [���ð�]				*/
	char high[8];			/*  [��,       ��]		*/
	char lowtime[6];		/*  [�����ð�]				*/
	char low[8];			/*  [����,       ��]		*/
	char openchange[8];		/*  [�ð����ϴ��]			*/
	char opendrate[6];		/*  [�ð����ϴ������]    */
	char highchange[8];		/*  [�����ϴ��]			*/
	char highdrate[6];		/*  [�����ϴ������]    */
	char lowchange[8];		/*  [�������ϴ��]			*/
	char lowdrate[6];		/*  [�������ϴ������]    */
	char cgubun[1];			/*  [ü�ᱸ��]				*/
	char cvolume[8];		/*  [ü�ᷮ, ��]			*/
	char volume[12];		/*  [�����ŷ���, ��]		*/
	char volchange[12];		/*  [�ŷ������ϴ��]		*/
	char voldrate[8];		/*  [�ŷ��������]			*/
	char voltrate[6];		/*  [�ŷ���ȸ����]			*/
	char value[12];			/*  [�����ŷ����]			*/
	char valchange[12];		/*  [�ŷ�������ϴ��]      */
	char valdrate[8];		/*  [�ŷ���ݵ����]		*/
	char mdvolume[12];		/*  [�ŵ����� ü�ᷮ]		*/
	char mdchecnt[8];		/*  [�ŵ����� ü��Ǽ�]     */
	char msvolume[12];		/*  [�ż����� ü�ᷮ]		*/
	char mschecnt[8];		/*  [�ż����� ü��Ǽ�]     */
	char cpower[9];			/*  [ü�ᰭ��]				*/
	char w_avrg[8];			/*  [���� ��հ�]			*/
	char offerho1[8];		/*  [�ŵ�ȣ��, ��]			*/
	char bidho1[8];			/*  [�ż�ȣ��, ��]			*/
	char status[2];			/*  [�屸�� ����]			*/
	char jnilvolume[12];	/*  [���ϵ��ð���ŷ���]    */
	char shcode[6];			/*  [�����ڵ�]				*/
} stEUREAL_KXH3;	//21

class CEUREAL_KXH3
{
public: //Variable
	CString m_szdgubun;			/*  [Data ����:]			*/
	CString m_szkshcode;		/*  [�����ڵ�]				*/
	CString m_szchetime;		/*  [ü��ð�]				*/
	CString m_szsign;			/*  [1:����]				*/
	CString m_szchange;			/*  [���ϴ��,   ��]		*/
	CString m_szdrate;			/*  [�����]				*/
	CString m_szprice;			/*  [���簡,     ��]		*/
	CString m_szopentime;		/*  [�ð��ð�]				*/
	CString m_szopen;			/*  [�ð�,       ��]		*/
	CString m_szhightime;		/*  [���ð�]				*/
	CString m_szhigh;			/*  [��,       ��]		*/
	CString m_szlowtime;		/*  [�����ð�]				*/
	CString m_szlow;			/*  [����,       ��]		*/
	CString m_szopenchange;		/*  [�ð����ϴ��]			*/
	CString m_szopendrate;		/*  [�ð����ϴ������]    */
	CString m_szhighchange;		/*  [�����ϴ��]			*/
	CString m_szhighdrate;		/*  [�����ϴ������]    */
	CString m_szlowchange;		/*  [�������ϴ��]			*/
	CString m_szlowdrate;		/*  [�������ϴ������]    */
	CString m_szcgubun;			/*  [ü�ᱸ��]				*/
	CString m_szcvolume;		/*  [ü�ᷮ, ��]			*/
	CString m_szvolume;			/*  [�����ŷ���, ��]		*/
	CString m_szvolchange;		/*  [�ŷ������ϴ��]		*/
	CString m_szvoldrate;		/*  [�ŷ��������]			*/
	CString m_szvoltrate;		/*  [�ŷ���ȸ����]			*/
	CString m_szvalue;			/*  [�����ŷ����]			*/
	CString m_szvalchange;		/*  [�ŷ�������ϴ��]      */
	CString m_szvaldrate;		/*  [�ŷ���ݵ����]		*/
	CString m_szmdvolume;		/*  [�ŵ����� ü�ᷮ]		*/
	CString m_szmdchecnt;		/*  [�ŵ����� ü��Ǽ�]     */
	CString m_szmsvolume;		/*  [�ż����� ü�ᷮ]		*/
	CString m_szmschecnt;		/*  [�ż����� ü��Ǽ�]     */
	CString m_szcpower;			/*  [ü�ᰭ��]				*/
	CString m_szw_avrg;			/*  [���� ��հ�]			*/
	CString m_szofferho1;		/*  [�ŵ�ȣ��, ��]			*/
	CString m_szbidho1;			/*  [�ż�ȣ��, ��]			*/
	CString m_szstatus;			/*  [�屸�� ����]			*/
	CString m_szjnilvolume;		/*  [���ϵ��ð���ŷ���]    */
	CString m_szshcode;			/*  [�����ڵ�]				*/

public:
	CEUREAL_KXH3()
	{
	}

	virtual void  Convert(stEUREAL_KXH3* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data ����:]			*/
		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [�����ڵ�]				*/
		DATA_MEMCOPY(sTmp, pInput, chetime);		/*  [ü��ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, sign);			/*  [1:����]				*/
		DATA_MEMCOPY(sTmp, pInput, change);			/*  [���ϴ��,   ��]		*/
		DATA_MEMCOPY(sTmp, pInput, drate);			/*  [�����]				*/
		DATA_MEMCOPY(sTmp, pInput, price);			/*  [���簡,     ��]		*/
		DATA_MEMCOPY(sTmp, pInput, opentime);		/*  [�ð��ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, open);			/*  [�ð�,       ��]		*/
		DATA_MEMCOPY(sTmp, pInput, hightime);		/*  [���ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, high);			/*  [��,       ��]		*/
		DATA_MEMCOPY(sTmp, pInput, lowtime);		/*  [�����ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, low);			/*  [����,       ��]		*/
		DATA_MEMCOPY(sTmp, pInput, openchange);		/*  [�ð����ϴ��]			*/
		DATA_MEMCOPY(sTmp, pInput, opendrate);		/*  [�ð����ϴ������]    */
		DATA_MEMCOPY(sTmp, pInput, highchange);		/*  [�����ϴ��]			*/
		DATA_MEMCOPY(sTmp, pInput, highdrate);		/*  [�����ϴ������]    */
		DATA_MEMCOPY(sTmp, pInput, lowchange);		/*  [�������ϴ��]			*/
		DATA_MEMCOPY(sTmp, pInput, lowdrate);		/*  [�������ϴ������]    */
		DATA_MEMCOPY(sTmp, pInput, cgubun);			/*  [ü�ᱸ��]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);		/*  [ü�ᷮ, ��]			*/
		DATA_MEMCOPY(sTmp, pInput, volume);			/*  [�����ŷ���, ��]		*/
		DATA_MEMCOPY(sTmp, pInput, volchange);		/*  [�ŷ������ϴ��]		*/
		DATA_MEMCOPY(sTmp, pInput, voldrate);		/*  [�ŷ��������]			*/
		DATA_MEMCOPY(sTmp, pInput, voltrate);		/*  [�ŷ���ȸ����]			*/
		DATA_MEMCOPY(sTmp, pInput, value);			/*  [�����ŷ����]			*/
		DATA_MEMCOPY(sTmp, pInput, valchange);		/*  [�ŷ�������ϴ��]      */
		DATA_MEMCOPY(sTmp, pInput, valdrate);		/*  [�ŷ���ݵ����]		*/
		DATA_MEMCOPY(sTmp, pInput, mdvolume);		/*  [�ŵ����� ü�ᷮ]		*/
		DATA_MEMCOPY(sTmp, pInput, mdchecnt);		/*  [�ŵ����� ü��Ǽ�]     */
		DATA_MEMCOPY(sTmp, pInput, msvolume);		/*  [�ż����� ü�ᷮ]		*/
		DATA_MEMCOPY(sTmp, pInput, mschecnt);		/*  [�ż����� ü��Ǽ�]     */
		DATA_MEMCOPY(sTmp, pInput, cpower);			/*  [ü�ᰭ��]				*/
		DATA_MEMCOPY(sTmp, pInput, w_avrg);			/*  [���� ��հ�]			*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [�ŵ�ȣ��, ��]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [�ż�ȣ��, ��]			*/
		DATA_MEMCOPY(sTmp, pInput, status);			/*  [�屸�� ����]			*/
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/*  [���ϵ��ð���ŷ���]    */
		DATA_MEMCOPY(sTmp, pInput, shcode);			/*  [�����ڵ�]				*/
	}
};

#endif // AFX_EUREAL_KXH3__H_C61A6AD4_A32A_4238_8D21_0E7DE88C9537_INCLUDED_
