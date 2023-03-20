#if !defined(AFX_EUREAL_IXIJ__H_20FBD731_704A_4a5b_BF46_064F9D0A6DE6_INCLUDED_)
#define AFX_EUREAL_IXIJ__H_20FBD731_704A_4a5b_BF46_064F9D0A6DE6_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     ����
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];		/*  [Data ����]				*/
	char		kupcode[3];		/*  [�����ڵ�]				*/
	char		time[6];		/*  [�ð�]					*/
	char		jisu[8];		/*  [����]					*/
	char		sign[1];		/*  [2:��� 3:���� 5:�϶�]  */
	char		change[8];		/*  [���Ϻ�]				*/
	char		drate[6];		/*  [�����]				*/
	char		cvolume[8];		/*  [ü�ᷮ]				*/
	char		volume[8];		/*  [�ŷ���]				*/
	char		volchange[8];	/*  [�ŷ������ϴ��]		*/
	char		voldrate[8];	/*  [�ŷ��������]			*/
	char		value[8];		/*  [�ŷ����]				*/
	char		upjo[4];		/*  [���� �����]			*/
	char		highjo[4];		/*  [��� �����]			*/
	char		unchgjo[4];		/*  [���� �����]			*/
	char		lowjo[4];		/*  [�϶� �����]			*/
	char		downjo[4];		/*  [���� �����]			*/
	char		upjrate[6];		/*  [��� �������]			*/
	char		openjisu[8];	/*  [�ð�����]				*/
	char		opentime[6];	/*  [�ð��ð�]				*/
	char		highjisu[8];	/*  [������]				*/
	char		hightime[6];	/*  [���ð�]				*/
	char		lowjisu[8];		/*  [��������]				*/
	char		lowtime[6];		/*  [�����ð�]				*/
	char		frgsvolume[8];	/*  [���μ��ż�����]		*/
	char		orgsvolume[8];	/*  [������ż�����]		*/
	char		frgsvalue[10];	/*  [���μ��ż��ݾ�]		*/
	char		orgsvalue[10];	/*  [������ż��ݾ�]		*/
	char		upcode[3];		/*  [�����ڵ�]				*/
} stEUREAL_IXIJ;	//101

class CEUREAL_IXIJ
{
public: //Variable
	CString m_szdgubun;			/*  [Data ����]				*/
	CString m_szkupcode;		/*  [�����ڵ�]				*/
	CString m_sztime;			/*  [�ð�]					*/
	CString m_szjisu;			/*  [����]					*/
	CString m_szsign;			/*  [2:��� 3:���� 5:�϶�]  */
	CString m_szchange;			/*  [���Ϻ�]				*/
	CString m_szdrate;			/*  [�����]				*/
	CString m_szcvolume;		/*  [ü�ᷮ]				*/
	CString m_szvolume;			/*  [�ŷ���]				*/
	CString m_szvolchange;		/*  [�ŷ������ϴ��]		*/
	CString m_szvoldrate;		/*  [�ŷ��������]			*/
	CString m_szvalue;			/*  [�ŷ����]				*/
	CString m_szupjo;			/*  [���� �����]			*/
	CString m_szhighjo;			/*  [��� �����]			*/
	CString m_szunchgjo;		/*  [���� �����]			*/
	CString m_szlowjo;			/*  [�϶� �����]			*/
	CString m_szdownjo;			/*  [���� �����]			*/
	CString m_szupjrate;		/*  [��� �������]			*/
	CString m_szopenjisu;		/*  [�ð�����]				*/
	CString m_szopentime;		/*  [�ð��ð�]				*/
	CString m_szhighjisu;		/*  [������]				*/
	CString m_szhightime;		/*  [���ð�]				*/
	CString m_szlowjisu;		/*  [��������]				*/
	CString m_szlowtime;		/*  [�����ð�]				*/
	CString m_szfrgsvolume;		/*  [���μ��ż�����]		*/
	CString m_szorgsvolume;		/*  [������ż�����]		*/
	CString m_szfrgsvalue;		/*  [���μ��ż��ݾ�]		*/
	CString m_szorgsvalue;		/*  [������ż��ݾ�]		*/
	CString m_szupcode;			/*  [�����ڵ�]				*/

public:
	CEUREAL_IXIJ()
	{
	}

	virtual void  Convert(stEUREAL_IXIJ* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [Data ����]				*/
		DATA_MEMCOPY(sTmp, pInput, kupcode);		/*  [�����ڵ�]				*/
		DATA_MEMCOPY(sTmp, pInput, time);			/*  [�ð�]					*/
		DATA_MEMCOPY(sTmp, pInput, jisu);			/*  [����]					*/
		DATA_MEMCOPY(sTmp, pInput, sign);			/*  [2:��� 3:���� 5:�϶�]  */
		DATA_MEMCOPY(sTmp, pInput, change);			/*  [���Ϻ�]				*/
		DATA_MEMCOPY(sTmp, pInput, drate);			/*  [�����]				*/
		DATA_MEMCOPY(sTmp, pInput, cvolume);		/*  [ü�ᷮ]				*/
		DATA_MEMCOPY(sTmp, pInput, volume);			/*  [�ŷ���]				*/
		DATA_MEMCOPY(sTmp, pInput, volchange);		/*  [�ŷ������ϴ��]		*/
		DATA_MEMCOPY(sTmp, pInput, voldrate);		/*  [�ŷ��������]			*/
		DATA_MEMCOPY(sTmp, pInput, value);			/*  [�ŷ����]				*/
		DATA_MEMCOPY(sTmp, pInput, upjo);			/*  [���� �����]			*/
		DATA_MEMCOPY(sTmp, pInput, highjo);			/*  [��� �����]			*/
		DATA_MEMCOPY(sTmp, pInput, unchgjo);		/*  [���� �����]			*/
		DATA_MEMCOPY(sTmp, pInput, lowjo);			/*  [�϶� �����]			*/
		DATA_MEMCOPY(sTmp, pInput, downjo);			/*  [���� �����]			*/
		DATA_MEMCOPY(sTmp, pInput, upjrate);		/*  [��� �������]			*/
		DATA_MEMCOPY(sTmp, pInput, openjisu);		/*  [�ð�����]				*/
		DATA_MEMCOPY(sTmp, pInput, opentime);		/*  [�ð��ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, highjisu);		/*  [������]				*/
		DATA_MEMCOPY(sTmp, pInput, hightime);		/*  [���ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, lowjisu);		/*  [��������]				*/
		DATA_MEMCOPY(sTmp, pInput, lowtime);		/*  [�����ð�]				*/
		DATA_MEMCOPY(sTmp, pInput, frgsvolume);		/*  [���μ��ż�����]		*/
		DATA_MEMCOPY(sTmp, pInput, orgsvolume);		/*  [������ż�����]		*/
		DATA_MEMCOPY(sTmp, pInput, frgsvalue);		/*  [���μ��ż��ݾ�]		*/
		DATA_MEMCOPY(sTmp, pInput, orgsvalue);		/*  [������ż��ݾ�]		*/
		DATA_MEMCOPY(sTmp, pInput, upcode);			/*  [�����ڵ�]				*/
	}
};

#endif // AFX_EUREAL_IXIJ__H_20FBD731_704A_4a5b_BF46_064F9D0A6DE6_INCLUDED_
