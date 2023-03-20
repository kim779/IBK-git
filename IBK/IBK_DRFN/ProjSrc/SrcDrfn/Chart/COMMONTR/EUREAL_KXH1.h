#if !defined(AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_)
#define AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_

// #include "../CommonTR/ChartTrAll.h"
// ���� : 2008.04.25(��)

#include "Data_Common.h"

//--------------------------------------------------------------------
//     �ֽ�ȣ��
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];			/*  [KRX ȣ���ܷ�]     */
	char		kshcode[6];			/*  [�����ڵ�]     */
	char		hotime[6];			/*  [ȣ���ð� HHMMSS]     */
	char		offerho1[8];		/*  [�ŵ�ȣ�� 1]     */
	char		bidho1[8];			/*  [�ż�ȣ�� 1]     */
	char		offerrem1[12];		/*  [�ŵ�ȣ�� �ܷ� 1]     */
	char		bidrem1[12];		/*  [�ż�ȣ�� �ܷ� 1]     */
	char		preoffercha1[12];	/*  [�����ŵ������� 1]     */
	char		prebidcha1[12];		/*  [�����ż������� 1]     */
	char		offerho2[8];		/*  [�ŵ�ȣ�� 2]     */
	char		bidho2[8];			/*  [�ż�ȣ�� 2]     */
	char		offerrem2[12];		/*  [�ŵ�ȣ�� �ܷ� 2]     */
	char		bidrem2[12];		/*  [�ż�ȣ�� �ܷ� 2]     */
	char		preoffercha2[12];	/*  [�����ŵ������� 2]     */
	char		prebidcha2[12];		/*  [�����ż������� 2]     */
	char		offerho3[8];		/*  [�ŵ�ȣ�� 3]     */
	char		bidho3[8];			/*  [�ż�ȣ�� 3]     */
	char		offerrem3[12];		/*  [�ŵ�ȣ�� �ܷ� 3]     */
	char		bidrem3[12];		/*  [�ż�ȣ�� �ܷ� 3]     */
	char		preoffercha3[12];	/*  [�����ŵ������� 3]     */
	char		prebidcha3[12];		/*  [�����ż������� 3]     */
	char		offerho4[8];		/*  [�ŵ�ȣ�� 4]     */
	char		bidho4[8];			/*  [�ż�ȣ�� 4]     */
	char		offerrem4[12];		/*  [�ŵ�ȣ�� �ܷ� 4]     */
	char		bidrem4[12];		/*  [�ż�ȣ�� �ܷ� 4]     */
	char		preoffercha4[12];	/*  [�����ŵ������� 5]     */
	char		prebidcha4[12];		/*  [�����ż������� 5]     */
	char		offerho5[8];		/*  [�ŵ�ȣ�� 5]     */
	char		bidho5[8];			/*  [�ż�ȣ�� 5]     */
	char		offerrem5[12];		/*  [�ŵ�ȣ�� �ܷ� 5]     */
	char		bidrem5[12];		/*  [�ż�ȣ�� �ܷ� 5]     */
	char		preoffercha5[12];	/*  [�����ŵ������� 5]     */
	char		prebidcha5[12];		/*  [�����ż������� 5]     */
	char		offerho6[8];		/*  [�ŵ�ȣ�� 6]     */
	char		bidho6[8];			/*  [�ż�ȣ�� 6]     */
	char		offerrem6[12];		/*  [�ŵ�ȣ�� �ܷ� 6]     */
	char		bidrem6[12];		/*  [�ż�ȣ�� �ܷ� 6]     */
	char		preoffercha6[12];	/*  [�����ŵ������� 6]     */
	char		prebidcha6[12];		/*  [�����ż������� 6]     */
	char		offerho7[8];		/*  [�ŵ�ȣ�� 7]     */
	char		bidho7[8];			/*  [�ż�ȣ�� 7]     */
	char		offerrem7[12];		/*  [�ŵ�ȣ�� �ܷ� 7]     */
	char		bidrem7[12];		/*  [�ż�ȣ�� �ܷ� 7]     */
	char		preoffercha7[12];	/*  [�����ŵ������� 7]     */
	char		prebidcha7[12];		/*  [�����ż������� 7]     */
	char		offerho8[8];		/*  [�ŵ�ȣ�� 8]     */
	char		bidho8[8];			/*  [�ż�ȣ�� 8]     */
	char		offerrem8[12];		/*  [�ŵ�ȣ�� �ܷ� 8]     */
	char		bidrem8[12];		/*  [�ż�ȣ�� �ܷ� 8]     */
	char		preoffercha8[12];	/*  [�����ŵ������� 8]     */
	char		prebidcha8[12];		/*  [�����ż������� 8]     */
	char		offerho9[8];		/*  [�ŵ�ȣ�� 9]     */
	char		bidho9[8];			/*  [�ż�ȣ�� 9]     */
	char		offerrem9[12];		/*  [�ŵ�ȣ�� �ܷ� 9]     */
	char		bidrem9[12];		/*  [�ż�ȣ�� �ܷ� 9]     */
	char		preoffercha9[12];	/*  [�����ŵ������� 9]     */
	char		prebidcha9[12];		/*  [�����ż������� 9]     */
	char		offerho10[8];		/*  [�ŵ�ȣ�� 10]     */
	char		bidho10[8];			/*  [�ż�ȣ�� 10]     */
	char		offerrem10[12];		/*  [�ŵ�ȣ�� �ܷ� 10]     */
	char		bidrem10[12];		/*  [�ż�ȣ�� �ܷ� 10]     */
	char		preoffercha10[12];	/*  [�����ŵ������� 10]     */
	char		prebidcha10[12];	/*  [�����ż������� 10]     */
	char		totofferrem[12];	/*  [�ŵ�ȣ���Ѽ���]     */
	char		totbidrem[12];		/*  [�ż�ȣ���Ѽ���]     */
	char		preoffercha[12];	/*  [�����ŵ�ȣ���Ѵ�����]     */
	char		prebidcha[12];		/*  [�����ż�ȣ���Ѵ�����]     */
	char		donsigubun[1];		/*  [����ȣ�� ����]     */
	char		shcode[6];			/*  [�����ڵ�]     */
} stEUREAL_KXH1;	//1

class CEUREAL_KXH1
{
public: //Variable
	CString		m_szdgubun;			/*  [KRX ȣ���ܷ�]			*/
	CString		m_szkshcode;		/*  [�����ڵ�]				*/
	CString		m_szhotime;			/*  [ȣ���ð� HHMMSS]		*/
	CString		m_szofferho1;		/*  [�ŵ�ȣ�� 1]			*/
	CString		m_szbidho1;			/*  [�ż�ȣ�� 1]			*/
	CString		m_szofferrem1;		/*  [�ŵ�ȣ�� �ܷ� 1]		*/
	CString		m_szbidrem1;		/*  [�ż�ȣ�� �ܷ� 1]		*/
	CString		m_szpreoffercha1;	/*  [�����ŵ������� 1]    */
	CString		m_szprebidcha1;		/*  [�����ż������� 1]    */
	CString		m_szofferho2;		/*  [�ŵ�ȣ�� 2]			*/
	CString		m_szbidho2;			/*  [�ż�ȣ�� 2]			*/
	CString		m_szofferrem2;		/*  [�ŵ�ȣ�� �ܷ� 2]		*/
	CString		m_szbidrem2;		/*  [�ż�ȣ�� �ܷ� 2]		*/
	CString		m_szpreoffercha2;	/*  [�����ŵ������� 2]    */
	CString		m_szprebidcha2;		/*  [�����ż������� 2]    */
	CString		m_szofferho3;		/*  [�ŵ�ȣ�� 3]			*/
	CString		m_szbidho3;			/*  [�ż�ȣ�� 3]			*/
	CString		m_szofferrem3;		/*  [�ŵ�ȣ�� �ܷ� 3]		*/
	CString		m_szbidrem3;		/*  [�ż�ȣ�� �ܷ� 3]		*/
	CString		m_szpreoffercha3;	/*  [�����ŵ������� 3]    */
	CString		m_szprebidcha3;		/*  [�����ż������� 3]    */
	CString		m_szofferho4;		/*  [�ŵ�ȣ�� 4]			*/
	CString		m_szbidho4;			/*  [�ż�ȣ�� 4]			*/
	CString		m_szofferrem4;		/*  [�ŵ�ȣ�� �ܷ� 4]		*/
	CString		m_szbidrem4;		/*  [�ż�ȣ�� �ܷ� 4]		*/
	CString		m_szpreoffercha4;	/*  [�����ŵ������� 5]    */
	CString		m_szprebidcha4;		/*  [�����ż������� 5]    */
	CString		m_szofferho5;		/*  [�ŵ�ȣ�� 5]			*/
	CString		m_szbidho5;			/*  [�ż�ȣ�� 5]			*/
	CString		m_szofferrem5;		/*  [�ŵ�ȣ�� �ܷ� 5]		*/
	CString		m_szbidrem5;		/*  [�ż�ȣ�� �ܷ� 5]		*/
	CString		m_szpreoffercha5;	/*  [�����ŵ������� 5]    */
	CString		m_szprebidcha5;		/*  [�����ż������� 5]    */
	CString		m_szofferho6;		/*  [�ŵ�ȣ�� 6]			*/
	CString		m_szbidho6;			/*  [�ż�ȣ�� 6]			*/
	CString		m_szofferrem6;		/*  [�ŵ�ȣ�� �ܷ� 6]		*/
	CString		m_szbidrem6;		/*  [�ż�ȣ�� �ܷ� 6]		*/
	CString		m_szpreoffercha6;	/*  [�����ŵ������� 6]    */
	CString		m_szprebidcha6;		/*  [�����ż������� 6]    */
	CString		m_szofferho7;		/*  [�ŵ�ȣ�� 7]			*/
	CString		m_szbidho7;			/*  [�ż�ȣ�� 7]			*/
	CString		m_szofferrem7;		/*  [�ŵ�ȣ�� �ܷ� 7]		*/
	CString		m_szbidrem7;		/*  [�ż�ȣ�� �ܷ� 7]		*/
	CString		m_szpreoffercha7;	/*  [�����ŵ������� 7]    */
	CString		m_szprebidcha7;		/*  [�����ż������� 7]    */
	CString		m_szofferho8;		/*  [�ŵ�ȣ�� 8]			*/
	CString		m_szbidho8;			/*  [�ż�ȣ�� 8]			*/
	CString		m_szofferrem8;		/*  [�ŵ�ȣ�� �ܷ� 8]		*/
	CString		m_szbidrem8;		/*  [�ż�ȣ�� �ܷ� 8]		*/
	CString		m_szpreoffercha8;	/*  [�����ŵ������� 8]    */
	CString		m_szprebidcha8;		/*  [�����ż������� 8]    */
	CString		m_szofferho9;		/*  [�ŵ�ȣ�� 9]			*/
	CString		m_szbidho9;			/*  [�ż�ȣ�� 9]			*/
	CString		m_szofferrem9;		/*  [�ŵ�ȣ�� �ܷ� 9]		*/
	CString		m_szbidrem9;		/*  [�ż�ȣ�� �ܷ� 9]		*/
	CString		m_szpreoffercha9;	/*  [�����ŵ������� 9]    */
	CString		m_szprebidcha9;		/*  [�����ż������� 9]    */
	CString		m_szofferho10;		/*  [�ŵ�ȣ�� 10]			*/
	CString		m_szbidho10;		/*  [�ż�ȣ�� 10]			*/
	CString		m_szofferrem10;		/*  [�ŵ�ȣ�� �ܷ� 10]		*/
	CString		m_szbidrem10;		/*  [�ż�ȣ�� �ܷ� 10]		*/
	CString		m_szpreoffercha10;	/*  [�����ŵ������� 10]   */
	CString		m_szprebidcha10;	/*  [�����ż������� 10]   */
	CString		m_sztotofferrem;	/*  [�ŵ�ȣ���Ѽ���]		*/
	CString		m_sztotbidrem;		/*  [�ż�ȣ���Ѽ���]		*/
	CString		m_szpreoffercha;	/*  [�����ŵ�ȣ���Ѵ�����]*/
	CString		m_szprebidcha;		/*  [�����ż�ȣ���Ѵ�����]*/
	CString		m_szdonsigubun;		/*  [����ȣ�� ����]			*/
	CString		m_szshcode;			/*  [�����ڵ�]				*/

public:
	CEUREAL_KXH1()
	{
	}

	virtual void  Convert(stEUREAL_KXH1* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);			/*  [KRX ȣ���ܷ�]			*/
		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [�����ڵ�]				*/
		DATA_MEMCOPY(sTmp, pInput, hotime);			/*  [ȣ���ð� HHMMSS]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [�ŵ�ȣ�� 1]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [�ż�ȣ�� 1]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem1);		/*  [�ŵ�ȣ�� �ܷ� 1]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem1);		/*  [�ż�ȣ�� �ܷ� 1]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha1);	/*  [�����ŵ������� 1]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha1);		/*  [�����ż������� 1]    */
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/*  [�ŵ�ȣ�� 2]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho2);			/*  [�ż�ȣ�� 2]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem2);		/*  [�ŵ�ȣ�� �ܷ� 2]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem2);		/*  [�ż�ȣ�� �ܷ� 2]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha2);	/*  [�����ŵ������� 2]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha2);		/*  [�����ż������� 2]    */
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/*  [�ŵ�ȣ�� 3]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho3);			/*  [�ż�ȣ�� 3]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem3);		/*  [�ŵ�ȣ�� �ܷ� 3]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem3);		/*  [�ż�ȣ�� �ܷ� 3]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha3);	/*  [�����ŵ������� 3]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha3);		/*  [�����ż������� 3]    */
		DATA_MEMCOPY(sTmp, pInput, offerho4);		/*  [�ŵ�ȣ�� 4]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho4);			/*  [�ż�ȣ�� 4]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem4);		/*  [�ŵ�ȣ�� �ܷ� 4]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem4);		/*  [�ż�ȣ�� �ܷ� 4]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha4);	/*  [�����ŵ������� 5]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha4);		/*  [�����ż������� 5]    */
		DATA_MEMCOPY(sTmp, pInput, offerho5);		/*  [�ŵ�ȣ�� 5]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho5);			/*  [�ż�ȣ�� 5]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem5);		/*  [�ŵ�ȣ�� �ܷ� 5]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem5);		/*  [�ż�ȣ�� �ܷ� 5]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha5);	/*  [�����ŵ������� 5]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha5);		/*  [�����ż������� 5]    */
		DATA_MEMCOPY(sTmp, pInput, offerho6);		/*  [�ŵ�ȣ�� 6]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho6);			/*  [�ż�ȣ�� 6]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem6);		/*  [�ŵ�ȣ�� �ܷ� 6]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem6);		/*  [�ż�ȣ�� �ܷ� 6]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha6);	/*  [�����ŵ������� 6]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha6);		/*  [�����ż������� 6]    */
		DATA_MEMCOPY(sTmp, pInput, offerho7);		/*  [�ŵ�ȣ�� 7]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho7);			/*  [�ż�ȣ�� 7]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem7);		/*  [�ŵ�ȣ�� �ܷ� 7]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem7);		/*  [�ż�ȣ�� �ܷ� 7]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha7);	/*  [�����ŵ������� 7]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha7);		/*  [�����ż������� 7]    */
		DATA_MEMCOPY(sTmp, pInput, offerho8);		/*  [�ŵ�ȣ�� 8]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho8);			/*  [�ż�ȣ�� 8]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem8);		/*  [�ŵ�ȣ�� �ܷ� 8]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem8);		/*  [�ż�ȣ�� �ܷ� 8]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha8);	/*  [�����ŵ������� 8]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha8);		/*  [�����ż������� 8]    */
		DATA_MEMCOPY(sTmp, pInput, offerho9);		/*  [�ŵ�ȣ�� 9]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho9);			/*  [�ż�ȣ�� 9]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem9);		/*  [�ŵ�ȣ�� �ܷ� 9]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem9);		/*  [�ż�ȣ�� �ܷ� 9]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha9);	/*  [�����ŵ������� 9]    */
		DATA_MEMCOPY(sTmp, pInput, prebidcha9);		/*  [�����ż������� 9]    */
		DATA_MEMCOPY(sTmp, pInput, offerho10);		/*  [�ŵ�ȣ�� 10]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho10);		/*  [�ż�ȣ�� 10]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem10);		/*  [�ŵ�ȣ�� �ܷ� 10]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem10);		/*  [�ż�ȣ�� �ܷ� 10]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha10);	/*  [�����ŵ������� 10]   */
		DATA_MEMCOPY(sTmp, pInput, prebidcha10);	/*  [�����ż������� 10]   */
		DATA_MEMCOPY(sTmp, pInput, totofferrem);	/*  [�ŵ�ȣ���Ѽ���]		*/
		DATA_MEMCOPY(sTmp, pInput, totbidrem);		/*  [�ż�ȣ���Ѽ���]		*/
		DATA_MEMCOPY(sTmp, pInput, preoffercha);	/*  [�����ŵ�ȣ���Ѵ�����]*/
		DATA_MEMCOPY(sTmp, pInput, prebidcha);		/*  [�����ż�ȣ���Ѵ�����]*/
		DATA_MEMCOPY(sTmp, pInput, donsigubun);		/*  [����ȣ�� ����]			*/
		DATA_MEMCOPY(sTmp, pInput, shcode);			/*  [�����ڵ�]				*/
	}
};


typedef struct 
{
	char		dgubun[4];			/*  [�����ð���ȣ���ܷ�]     */
	char		kshcode[6];			/*  [�����ڵ�]     */
	char		hotime[6];			/*  [ȣ���ð�]     */
	char		tmofferrem[12];		/*  [�ð��ܸŵ��ܷ�]     */
	char		tmbidrem[12];		/*  [�ð��ܸż��ܷ�]     */
	char		pretmoffercha[12];	/*  [�ð��ܸŵ����� �������]     */
	char		pretmbidcha[12];	/*  [�ð��ܸż����� �������]     */
	char		shcode[6];			/*  [�����ڵ�]     */
} EUREAL_KXH2;	//3

#endif // AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_
