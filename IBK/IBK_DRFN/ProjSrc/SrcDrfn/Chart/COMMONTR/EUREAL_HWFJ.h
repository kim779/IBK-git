#if !defined(AFX_EUREAL_HWFJ__H_2D690C56_5D98_4d7d_8088_F3A1E2FA6F84_INCLUDED_)
#define AFX_EUREAL_HWFJ__H_2D690C56_5D98_4d7d_8088_F3A1E2FA6F84_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     �ؿ�
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];		/*  [�����ͱ���]			*/
	char		symbol[10];		/*  [�ɺ��ڵ�]				*/
	char		date[8];		/*  [����(��������)]		*/
	char		time[10];		/*  [�ð�(�����ð�)]		*/
	char		kodate[8];		/*  [�ѱ�����]				*/
	char		kotime[10];		/*  [�ѱ��ð�]				*/
	char		open[8];		/*  [�ð�]					*/
	char		high[8];		/*  [��]					*/
	char		low[8];			/*  [����]					*/
	char		price[8];		/*  [���簡]				*/
	char		sign[1];		/*  [���ϴ�񱸺�]			*/
	char		change[8];		/*  [���ϴ��]				*/
	char		drate[6];		/*  [�����]				*/
	char		bid[8];			/*  [�ż�ȣ��]				*/
	char		bidrem[9];		/*  [�ż��ܷ�]				*/
	char		offer[8];		/*  [�ŵ�ȣ��]				*/
	char		offerrem[9];	/*  [�ŵ��ܷ�]				*/
	char		cvolume[9];		/*  [ü�����]				*/
	char		volume[9];		/*  [�ŷ���]				*/
} stEUREAL_HWFJ;

class CEUREAL_HWFJ
{
public: //Variable
	CString		m_szdgubun;		/*  [�����ͱ���]			*/
	CString		m_szsymbol;		/*  [�ɺ��ڵ�]				*/
	CString		m_szdate;		/*  [����(��������)]		*/
	CString		m_sztime;		/*  [�ð�(�����ð�)]		*/
	CString		m_szkodate;		/*  [�ѱ�����]				*/
	CString		m_szkotime;		/*  [�ѱ��ð�]				*/
	CString		m_szopen;		/*  [�ð�]					*/
	CString		m_szhigh;		/*  [��]					*/
	CString		m_szlow;		/*  [����]					*/
	CString		m_szprice;		/*  [���簡]				*/
	CString		m_szsign;		/*  [���ϴ�񱸺�]			*/
	CString		m_szchange;		/*  [���ϴ��]				*/
	CString		m_szdrate;		/*  [�����]				*/
	CString		m_szbid;		/*  [�ż�ȣ��]				*/
	CString		m_szbidrem;		/*  [�ż��ܷ�]				*/
	CString		m_szoffer;		/*  [�ŵ�ȣ��]				*/
	CString		m_szofferrem;	/*  [�ŵ��ܷ�]				*/
	CString		m_szcvolume;	/*  [ü�����]				*/
	CString		m_szvolume;		/*  [�ŷ���]				*/

public:
	CEUREAL_HWFJ()
	{
	}

	virtual void  Convert(stEUREAL_HWFJ* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);		/*  [�����ͱ���]			*/
		DATA_MEMCOPY(sTmp, pInput, symbol);		/*  [�ɺ��ڵ�]				*/
		DATA_MEMCOPY(sTmp, pInput, date);		/*  [����(��������)]		*/
		DATA_MEMCOPY(sTmp, pInput, time);		/*  [�ð�(�����ð�)]		*/
		DATA_MEMCOPY(sTmp, pInput, kodate);		/*  [�ѱ�����]				*/
		DATA_MEMCOPY(sTmp, pInput, kotime);		/*  [�ѱ��ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, open);		/*  [�ð�]					*/
		DATA_MEMCOPY(sTmp, pInput, high);		/*  [��]					*/
		DATA_MEMCOPY(sTmp, pInput, low);		/*  [����]					*/
		DATA_MEMCOPY(sTmp, pInput, price);		/*  [���簡]				*/
		DATA_MEMCOPY(sTmp, pInput, sign);		/*  [���ϴ�񱸺�]			*/
		DATA_MEMCOPY(sTmp, pInput, change);		/*  [���ϴ��]				*/
		DATA_MEMCOPY(sTmp, pInput, drate);		/*  [�����]				*/
		DATA_MEMCOPY(sTmp, pInput, bid);		/*  [�ż�ȣ��]				*/
		DATA_MEMCOPY(sTmp, pInput, bidrem);		/*  [�ż��ܷ�]				*/
		DATA_MEMCOPY(sTmp, pInput, offer);		/*  [�ŵ�ȣ��]				*/
		DATA_MEMCOPY(sTmp, pInput, offerrem);	/*  [�ŵ��ܷ�]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);	/*  [ü�����]				*/
		DATA_MEMCOPY(sTmp, pInput, volume);		/*  [�ŷ���]				*/
	}
};

#endif // AFX_EUREAL_HWFJ__H_2D690C56_5D98_4d7d_8088_F3A1E2FA6F84_INCLUDED_
