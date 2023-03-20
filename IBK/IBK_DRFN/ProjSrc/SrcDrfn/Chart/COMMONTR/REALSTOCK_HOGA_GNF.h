#include "Data_Common.h"

//--------------------------------------------------------------------
//     �ֽ�
//--------------------------------------------------------------------
typedef struct 
{
	char		dgubun[4];		/*  [Data ����: CAH0]     */
	char		jmcode[10];		/*  [�����ڵ�]     */
	char		chetime[6];		/*  [ȣ���ð� HHMMSS]     */
	char		offerho1[10];		/*  [�켱�ŵ�ȣ��]     */
	char		offerho1_d[15];		/*  [������ȯ�� �켱�ŵ�ȣ��]     */
	char		bidho1[10];		/*  [�켱�ż�ȣ��]     */
	char		bidho1_d[15];		/*  [������ȯ�� �켱�ż�ȣ��]     */
	char		offerrem[8];		/*  [�켱�ŵ��ܷ�]     */
	char		bidrem[8];		/*  [�켱�ż��ܷ�]     */
	char		kjmcode[10];		/*  [�����ڵ�]     */

} STREAL_GNF_STOCK_HOGA;



class CREAL_GNF_STOCK_HOGA
{
public: //Variable
	CString m_szdgubun;		/*  [Data ����: CAH0]     */
	CString m_szjmcode;		/*  [�����ڵ�]     */
	CString m_szchetime;		/*  [ȣ���ð� HHMMSS]     */
	CString m_szofferho1;		/*  [�켱�ŵ�ȣ��]     */
	CString m_szofferho1_d;		/*  [������ȯ�� �켱�ŵ�ȣ��]     */
	CString m_szbidho1;		/*  [�켱�ż�ȣ��]     */
	CString m_szbidho1_d;		/*  [������ȯ�� �켱�ż�ȣ��]     */
	CString m_szofferrem;		/*  [�켱�ŵ��ܷ�]     */
	CString m_szbidrem;		/*  [�켱�ż��ܷ�]     */
	CString m_szkjmcode;		/*  [�����ڵ�]     */
public:
	CREAL_GNF_STOCK_HOGA()
	{
	}

	virtual void  Convert(STREAL_GNF_STOCK_HOGA* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun);		
		DATA_MEMCOPY(sTmp, pInput, jmcode);		
		DATA_MEMCOPY(sTmp, pInput, chetime);		
		DATA_MEMCOPY(sTmp, pInput, offerho1);	
		DATA_MEMCOPY(sTmp, pInput, offerho1_d);	
		DATA_MEMCOPY(sTmp, pInput, bidho1);		
		DATA_MEMCOPY(sTmp, pInput, bidho1_d);	
		DATA_MEMCOPY(sTmp, pInput, offerrem);	
		DATA_MEMCOPY(sTmp, pInput, bidrem);		
		DATA_MEMCOPY(sTmp, pInput, kjmcode);		
	}
};


