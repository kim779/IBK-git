#if !defined(AFX_EUREAL_KXYS__H_F9BA4E10_E4F6_40d8_8FC9_0F6E697669AB_INCLUDED_)
#define AFX_EUREAL_KXYS__H_F9BA4E10_E4F6_40d8_8FC9_0F6E697669AB_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     ����ü��
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [KRX ����ü��]     */
	char		kshcode[6];			/*  [�����ڵ�]     */
	char		hotime[6];			/*  [ȣ���ð� HHMMSS]     */
	char		yeprice[8];			/*  [����ü�ᰡ��]     */
	char		preysign[1];		/*  [����ü�ᰡ ��񱸺�]     */
	char		preychange[8];		/*  [����ü�ᰡ ���]     */
	char		preydrate[6];		/*  [����ü�ᰡ �����]     */
	char		jnilysign[1];		/*  [����ü�ᰡ �������� ��񱸺�]     */
	char		jnilychange[8];		/*  [����ü�ᰡ �������� ���]     */
	char		jnilydrate[6];		/*  [����ü�ᰡ �������� �����]     */
	char		yecvolume[8];		/*  [����ü�ắ����]     */
	char		yevolume[12];		/*  [����ü�����]     */
	char		yvolchange[12];		/*  [����ü����� ���ϰŷ��� ���]     */
	char		yofferho0[8];		/*  [����ŵ�ȣ��]     */
	char		ybidho0[8];			/*  [����ż�ȣ��]     */
	char		yofferrem0[12];		/*  [����ŵ�ȣ������]     */
	char		ybidrem0[12];		/*  [����ż�ȣ������]     */
	char		ypreoffercha0[12];	/*  [���������ŵ�������]     */
	char		yprebidcha0[12];	/*  [���������ż�������]    */
	char		shcode[6];			/*  [�����ڵ�]     */
} stEUREAL_KXYS;	//23

class CEUREAL_KXYS
{
public: //Variable
	CString		m_szdgubun;			/*  [KRX ����ü��]     */
	CString		m_szkshcode;		/*  [�����ڵ�]     */
	CString		m_szhotime;			/*  [ȣ���ð� HHMMSS]     */
	CString		m_szyeprice;		/*  [����ü�ᰡ��]     */
	CString		m_szpreysign;		/*  [����ü�ᰡ ��񱸺�]     */
	CString		m_szpreychange;		/*  [����ü�ᰡ ���]     */
	CString		m_szpreydrate;		/*  [����ü�ᰡ �����]     */
	CString		m_szjnilysign;		/*  [����ü�ᰡ �������� ��񱸺�]     */
	CString		m_szjnilychange;	/*  [����ü�ᰡ �������� ���]     */
	CString		m_szjnilydrate;		/*  [����ü�ᰡ �������� �����]     */
	CString		m_szyecvolume;		/*  [����ü�ắ����]     */
	CString		m_szyevolume;		/*  [����ü�����]     */
	CString		m_szyvolchange;		/*  [����ü����� ���ϰŷ��� ���]     */
	CString		m_szyofferho0;		/*  [����ŵ�ȣ��]     */
	CString		m_szybidho0;		/*  [����ż�ȣ��]     */
	CString		m_szyofferrem0;		/*  [����ŵ�ȣ������]     */
	CString		m_szybidrem0;		/*  [����ż�ȣ������]     */
	CString		m_szypreoffercha0;	/*  [���������ŵ�������]     */
	CString		m_szyprebidcha0;	/*  [���������ż�������]    */
	CString		m_szshcode;			/*  [�����ڵ�]     */

public:
	CEUREAL_KXYS()
	{
	}

	virtual void  Convert(stEUREAL_KXYS* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [KRX ����ü��]     */
		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [�����ڵ�]     */
		DATA_MEMCOPY(sTmp, pInput, hotime);			/*  [ȣ���ð� HHMMSS]     */
		DATA_MEMCOPY(sTmp, pInput, yeprice);		/*  [����ü�ᰡ��]     */
		DATA_MEMCOPY(sTmp, pInput, preysign);		/*  [����ü�ᰡ ��񱸺�]     */
		DATA_MEMCOPY(sTmp, pInput, preychange);		/*  [����ü�ᰡ ���]     */
		DATA_MEMCOPY(sTmp, pInput, preydrate);		/*  [����ü�ᰡ �����]     */
		DATA_MEMCOPY(sTmp, pInput, jnilysign);		/*  [����ü�ᰡ �������� ��񱸺�]     */
		DATA_MEMCOPY(sTmp, pInput, jnilychange);	/*  [����ü�ᰡ �������� ���]     */
		DATA_MEMCOPY(sTmp, pInput, jnilydrate);		/*  [����ü�ᰡ �������� �����]     */
		DATA_MEMCOPY(sTmp, pInput, yecvolume);		/*  [����ü�ắ����]     */
		DATA_MEMCOPY(sTmp, pInput, yevolume);		/*  [����ü�����]     */
		DATA_MEMCOPY(sTmp, pInput, yvolchange);		/*  [����ü����� ���ϰŷ��� ���]     */
		DATA_MEMCOPY(sTmp, pInput, yofferho0);		/*  [����ŵ�ȣ��]     */
		DATA_MEMCOPY(sTmp, pInput, ybidho0);		/*  [����ż�ȣ��]     */
		DATA_MEMCOPY(sTmp, pInput, yofferrem0);		/*  [����ŵ�ȣ������]     */
		DATA_MEMCOPY(sTmp, pInput, ybidrem0);		/*  [����ż�ȣ������]     */
		DATA_MEMCOPY(sTmp, pInput, ypreoffercha0);	/*  [���������ŵ�������]     */
		DATA_MEMCOPY(sTmp, pInput, yprebidcha0);	/*  [���������ż�������]    */
		DATA_MEMCOPY(sTmp, pInput, shcode);			/*  [�����ڵ�]     */
	}
};

#endif // AFX_EUREAL_KXYS__H_F9BA4E10_E4F6_40d8_8FC9_0F6E697669AB_INCLUDED_
