#if !defined(AFX_REAL_CAH1__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_)
#define AFX_REAL_CAH1__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_

#include "Data_Common.h"

//--------------------------------------------------------------------
//     ��Ÿ
//--------------------------------------------------------------------

typedef struct {
	char		dgubun[4];		/* Data ���� */
	char		kjmcode[10];		/* �����ڵ� */
	char		hotime[6];		/* ȣ���ð� */
	char		mdhoga1[10];		/* �ŵ�ȣ�� 1 */
	char		mshoga1[10];		/* �ż�ȣ�� 1 */
	char		mdhorem1[8];		/* �ŵ�ȣ�� �ܷ� 1 */
	char		mshorem1[8];		/* �ż�ȣ�� �ܷ� 1 */
	char		mdhocnt1[6];		/* �ŵ�ȣ�� �Ǽ� 1 */
	char		mshocnt1[6];		/* �ż�ȣ�� �Ǽ� 1 */
	char		premdhocha1[6];		/* �����ŵ������� 1 */
	char		premshocha1[6];		/* �����ż������� 1 */
	char		mdhoga2[10];		/* �ŵ�ȣ�� 2 */
	char		mshoga2[10];		/* �ż�ȣ�� 2 */
	char		mdhorem2[8];		/* �ŵ�ȣ�� �ܷ� 2 */
	char		mshorem2[8];		/* �ż�ȣ�� �ܷ� 2 */
	char		mdhocnt2[6];		/* �ŵ�ȣ�� �Ǽ� 2 */
	char		mshocnt2[6];		/* �ż�ȣ�� �Ǽ� 2 */
	char		premdhocha2[6];		/* �����ŵ������� 2 */
	char		premshocha2[6];		/* �����ż������� 2 */
	char		mdhoga3[10];		/* �ŵ�ȣ�� 3 */
	char		mshoga3[10];		/* �ż�ȣ�� 3 */
	char		mdhorem3[8];		/* �ŵ�ȣ�� �ܷ� 3 */
	char		mshorem3[8];		/* �ż�ȣ�� �ܷ� 3 */
	char		mdhocnt3[6];		/* �ŵ�ȣ�� �Ǽ� 3 */
	char		mshocnt3[6];		/* �ż�ȣ�� �Ǽ� 3 */
	char		premdhocha3[6];		/* �����ŵ������� 3 */
	char		premshocha3[6];		/* �����ż������� 3 */
	char		mdhoga4[10];		/* �ŵ�ȣ�� 4 */
	char		mshoga4[10];		/* �ż�ȣ�� 4 */
	char		mdhorem4[8];		/* �ŵ�ȣ�� �ܷ� 4 */
	char		mshorem4[8];		/* �ż�ȣ�� �ܷ� 4 */
	char		mdhocnt4[6];		/* �ŵ�ȣ�� �Ǽ� 4 */
	char		mshocnt4[6];		/* �ż�ȣ�� �Ǽ� 4 */
	char		premdhocha4[6];		/* �����ŵ������� 4 */
	char		premshocha4[6];		/* �����ż������� 4 */
	char		mdhoga5[10];		/* �ŵ�ȣ�� 5 */
	char		mshoga5[10];		/* �ż�ȣ�� 5 */
	char		mdhorem5[8];		/* �ŵ�ȣ�� �ܷ� 5 */
	char		mshorem5[8];		/* �ż�ȣ�� �ܷ� 5 */
	char		mdhocnt5[6];		/* �ŵ�ȣ�� �Ǽ� 5 */
	char		mshocnt5[6];		/* �ż�ȣ�� �Ǽ� 5 */
	char		premdhocha5[6];		/* �����ŵ������� 5 */
	char		premshocha5[6];		/* �����ż������� 5 */
	char		totmdhorem[10];		/* �� �ŵ�ȣ�� �ܷ� */
	char		totmshorem[10];		/* �� �ż�ȣ�� �ܷ� */
	char		totmdhocnt[8];		/* �� �ŵ�ȣ�� �Ǽ� */
	char		totmshocnt[8];		/* �� �ż�ȣ�� �Ǽ� */
	char		totpremdhocha[8];		/* �� �����ŵ������� */
	char		totpremshocha[8];		/* �� �����ż������� */
	char		jmcode[10];		/* �����ڵ� */

} stEUREAL_CAH1;	//67

class CREAL_CAH1
{
public: //Variable
	CString     m_szdgubun;				/* Data ���� */
	CString     m_szkjmcode;			/* �����ڵ� */
	CString     m_szhotime;				/* ȣ���ð� */
	CString     m_szmdhoga1;			/* �ŵ�ȣ�� 1 */
	CString     m_szmshoga1;			/* �ż�ȣ�� 1 */
	CString     m_szmdhorem1;			/* �ŵ�ȣ�� �ܷ� 1 */
	CString     m_szmshorem1;			/* �ż�ȣ�� �ܷ� 1 */
	CString     m_szmdhocnt1;			/* �ŵ�ȣ�� �Ǽ� 1 */
	CString     m_szmshocnt1;			/* �ż�ȣ�� �Ǽ� 1 */
	CString     m_szpremdhocha1;		/* �����ŵ������� 1 */
	CString     m_szpremshocha1;		/* �����ż������� 1 */
	CString     m_szmdhoga2;			/* �ŵ�ȣ�� 2 */
	CString     m_szmshoga2;			/* �ż�ȣ�� 2 */
	CString     m_szmdhorem2;			/* �ŵ�ȣ�� �ܷ� 2 */
	CString     m_szmshorem2;			/* �ż�ȣ�� �ܷ� 2 */
	CString     m_szmdhocnt2;			/* �ŵ�ȣ�� �Ǽ� 2 */
	CString     m_szmshocnt2;			/* �ż�ȣ�� �Ǽ� 2 */
	CString     m_szpremdhocha2;		/* �����ŵ������� 2 */
	CString     m_szpremshocha2;		/* �����ż������� 2 */
	CString     m_szmdhoga3;			/* �ŵ�ȣ�� 3 */
	CString     m_szmshoga3;			/* �ż�ȣ�� 3 */
	CString     m_szmdhorem3;			/* �ŵ�ȣ�� �ܷ� 3 */
	CString     m_szmshorem3;			/* �ż�ȣ�� �ܷ� 3 */
	CString     m_szmdhocnt3;			/* �ŵ�ȣ�� �Ǽ� 3 */
	CString     m_szmshocnt3;			/* �ż�ȣ�� �Ǽ� 3 */
	CString     m_szpremdhocha3;		/* �����ŵ������� 3 */
	CString     m_szpremshocha3;		/* �����ż������� 3 */
	CString     m_szmdhoga4;			/* �ŵ�ȣ�� 4 */
	CString     m_szmshoga4;			/* �ż�ȣ�� 4 */
	CString     m_szmdhorem4;			/* �ŵ�ȣ�� �ܷ� 4 */
	CString     m_szmshorem4;			/* �ż�ȣ�� �ܷ� 4 */
	CString     m_szmdhocnt4;			/* �ŵ�ȣ�� �Ǽ� 4 */
	CString     m_szmshocnt4;			/* �ż�ȣ�� �Ǽ� 4 */
	CString     m_szpremdhocha4;		/* �����ŵ������� 4 */
	CString     m_szpremshocha4;		/* �����ż������� 4 */
	CString     m_szmdhoga5;			/* �ŵ�ȣ�� 5 */
	CString     m_szmshoga5;			/* �ż�ȣ�� 5 */
	CString     m_szmdhorem5;			/* �ŵ�ȣ�� �ܷ� 5 */
	CString     m_szmshorem5;			/* �ż�ȣ�� �ܷ� 5 */
	CString     m_szmdhocnt5;			/* �ŵ�ȣ�� �Ǽ� 5 */
	CString     m_szmshocnt5;			/* �ż�ȣ�� �Ǽ� 5 */
	CString     m_szpremdhocha5;		/* �����ŵ������� 5 */
	CString     m_szpremshocha5;		/* �����ż������� 5 */
	CString     m_sztotmdhorem;			/* �� �ŵ�ȣ�� �ܷ� */
	CString     m_sztotmshorem;			/* �� �ż�ȣ�� �ܷ� */
	CString     m_sztotmdhocnt;			/* �� �ŵ�ȣ�� �Ǽ� */
	CString     m_sztotmshocnt;			/* �� �ż�ȣ�� �Ǽ� */
	CString     m_sztotpremdhocha;		/* �� �����ŵ������� */
	CString     m_sztotpremshocha;		/* �� �����ż������� */
	CString     m_szjmcode;				/* �����ڵ� */


public:
	CREAL_CAH1()
	{
	}

	virtual void  Convert(stEUREAL_CAH1* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, dgubun		);/* Data ���� */
		DATA_MEMCOPY(sTmp, pInput, kjmcode		);/* �����ڵ� */
		DATA_MEMCOPY(sTmp, pInput, hotime		);/* ȣ���ð� */
		DATA_MEMCOPY(sTmp, pInput, mdhoga1		);/* �ŵ�ȣ�� 1 */
		DATA_MEMCOPY(sTmp, pInput, mshoga1		);/* �ż�ȣ�� 1 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem1		);/* �ŵ�ȣ�� �ܷ� 1 */
		DATA_MEMCOPY(sTmp, pInput, mshorem1		);/* �ż�ȣ�� �ܷ� 1 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt1		);/* �ŵ�ȣ�� �Ǽ� 1 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt1		);/* �ż�ȣ�� �Ǽ� 1 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha1	);/* �����ŵ������� 1 */
		DATA_MEMCOPY(sTmp, pInput, premshocha1	);/* �����ż������� 1 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga2		);/* �ŵ�ȣ�� 2 */
		DATA_MEMCOPY(sTmp, pInput, mshoga2		);/* �ż�ȣ�� 2 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem2		);/* �ŵ�ȣ�� �ܷ� 2 */
		DATA_MEMCOPY(sTmp, pInput, mshorem2		);/* �ż�ȣ�� �ܷ� 2 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt2		);/* �ŵ�ȣ�� �Ǽ� 2 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt2		);/* �ż�ȣ�� �Ǽ� 2 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha2	);/* �����ŵ������� 2 */
		DATA_MEMCOPY(sTmp, pInput, premshocha2	);/* �����ż������� 2 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga3		);/* �ŵ�ȣ�� 3 */
		DATA_MEMCOPY(sTmp, pInput, mshoga3		);/* �ż�ȣ�� 3 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem3		);/* �ŵ�ȣ�� �ܷ� 3 */
		DATA_MEMCOPY(sTmp, pInput, mshorem3		);/* �ż�ȣ�� �ܷ� 3 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt3		);/* �ŵ�ȣ�� �Ǽ� 3 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt3		);/* �ż�ȣ�� �Ǽ� 3 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha3	);/* �����ŵ������� 3 */
		DATA_MEMCOPY(sTmp, pInput, premshocha3	);/* �����ż������� 3 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga4		);/* �ŵ�ȣ�� 4 */
		DATA_MEMCOPY(sTmp, pInput, mshoga4		);/* �ż�ȣ�� 4 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem4		);/* �ŵ�ȣ�� �ܷ� 4 */
		DATA_MEMCOPY(sTmp, pInput, mshorem4		);/* �ż�ȣ�� �ܷ� 4 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt4		);/* �ŵ�ȣ�� �Ǽ� 4 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt4		);/* �ż�ȣ�� �Ǽ� 4 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha4	);/* �����ŵ������� 4 */
		DATA_MEMCOPY(sTmp, pInput, premshocha4	);/* �����ż������� 4 */
		DATA_MEMCOPY(sTmp, pInput, mdhoga5		);/* �ŵ�ȣ�� 5 */
		DATA_MEMCOPY(sTmp, pInput, mshoga5		);/* �ż�ȣ�� 5 */
		DATA_MEMCOPY(sTmp, pInput, mdhorem5		);/* �ŵ�ȣ�� �ܷ� 5 */
		DATA_MEMCOPY(sTmp, pInput, mshorem5		);/* �ż�ȣ�� �ܷ� 5 */
		DATA_MEMCOPY(sTmp, pInput, mdhocnt5		);/* �ŵ�ȣ�� �Ǽ� 5 */
		DATA_MEMCOPY(sTmp, pInput, mshocnt5		);/* �ż�ȣ�� �Ǽ� 5 */
		DATA_MEMCOPY(sTmp, pInput, premdhocha5	);/* �����ŵ������� 5 */
		DATA_MEMCOPY(sTmp, pInput, premshocha5	);/* �����ż������� 5 */
		DATA_MEMCOPY(sTmp, pInput, totmdhorem	);/* �� �ŵ�ȣ�� �ܷ� */
		DATA_MEMCOPY(sTmp, pInput, totmshorem	);/* �� �ż�ȣ�� �ܷ� */
		DATA_MEMCOPY(sTmp, pInput, totmdhocnt	);/* �� �ŵ�ȣ�� �Ǽ� */
		DATA_MEMCOPY(sTmp, pInput, totmshocnt	);/* �� �ż�ȣ�� �Ǽ� */
		DATA_MEMCOPY(sTmp, pInput, totpremdhocha);/* �� �����ŵ������� */
		DATA_MEMCOPY(sTmp, pInput, totpremshocha);/* �� �����ż������� */
		DATA_MEMCOPY(sTmp, pInput, jmcode		);/* �����ڵ� */
	}
};

#endif // AFX_REAL_CAH1__H_1A69CAB4_990E_4c28_9C85_63E7A39B8341_INCLUDED_
