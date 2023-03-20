#if !defined(AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_)
#define AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// KRX ȣ���ܷ�(KXH1) ( KXH1,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_KXH1     "KXH1"

// �Է�
typedef struct _KXH1_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
} KXH1_InBlock, *LPKXH1_InBlock;
#define NAME_KXH1_InBlock     "InBlock"

// ���
typedef struct _KXH1_OutBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ���ð�
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] �ŵ�ȣ��1
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] �ż�ȣ��1
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] �ŵ�ȣ���ܷ�1
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] �ż�ȣ���ܷ�1
    char    preoffercha1        [  12];    char    _preoffercha1        ;    // [long  ,   12] �����ŵ�������1
    char    prebidcha1          [  12];    char    _prebidcha1          ;    // [long  ,   12] �����ż�������1
    char    offerho2            [   8];    char    _offerho2            ;    // [long  ,    8] �ŵ�ȣ��2
    char    bidho2              [   8];    char    _bidho2              ;    // [long  ,    8] �ż�ȣ��2
    char    offerrem2           [  12];    char    _offerrem2           ;    // [long  ,   12] �ŵ�ȣ���ܷ�2
    char    bidrem2             [  12];    char    _bidrem2             ;    // [long  ,   12] �ż�ȣ���ܷ�2
    char    preoffercha2        [  12];    char    _preoffercha2        ;    // [long  ,   12] �����ŵ�������2
    char    prebidcha2          [  12];    char    _prebidcha2          ;    // [long  ,   12] �����ż�������2
    char    offerho3            [   8];    char    _offerho3            ;    // [long  ,    8] �ŵ�ȣ��3
    char    bidho3              [   8];    char    _bidho3              ;    // [long  ,    8] �ż�ȣ��3
    char    offerrem3           [  12];    char    _offerrem3           ;    // [long  ,   12] �ŵ�ȣ���ܷ�3
    char    bidrem3             [  12];    char    _bidrem3             ;    // [long  ,   12] �ż�ȣ���ܷ�3
    char    preoffercha3        [  12];    char    _preoffercha3        ;    // [long  ,   12] �����ŵ�������3
    char    prebidcha3          [  12];    char    _prebidcha3          ;    // [long  ,   12] �����ż�������3
    char    offerho4            [   8];    char    _offerho4            ;    // [long  ,    8] �ŵ�ȣ��4
    char    bidho4              [   8];    char    _bidho4              ;    // [long  ,    8] �ż�ȣ��4
    char    offerrem4           [  12];    char    _offerrem4           ;    // [long  ,   12] �ŵ�ȣ���ܷ�4
    char    bidrem4             [  12];    char    _bidrem4             ;    // [long  ,   12] �ż�ȣ���ܷ�4
    char    preoffercha4        [  12];    char    _preoffercha4        ;    // [long  ,   12] �����ŵ�������5
    char    prebidcha4          [  12];    char    _prebidcha4          ;    // [long  ,   12] �����ż�������5
    char    offerho5            [   8];    char    _offerho5            ;    // [long  ,    8] �ŵ�ȣ��5
    char    bidho5              [   8];    char    _bidho5              ;    // [long  ,    8] �ż�ȣ��5
    char    offerrem5           [  12];    char    _offerrem5           ;    // [long  ,   12] �ŵ�ȣ���ܷ�5
    char    bidrem5             [  12];    char    _bidrem5             ;    // [long  ,   12] �ż�ȣ���ܷ�5
    char    preoffercha5        [  12];    char    _preoffercha5        ;    // [long  ,   12] �����ŵ�������5
    char    prebidcha5          [  12];    char    _prebidcha5          ;    // [long  ,   12] �����ż�������5
    char    offerho6            [   8];    char    _offerho6            ;    // [long  ,    8] �ŵ�ȣ��6
    char    bidho6              [   8];    char    _bidho6              ;    // [long  ,    8] �ż�ȣ��6
    char    offerrem6           [  12];    char    _offerrem6           ;    // [long  ,   12] �ŵ�ȣ���ܷ�6
    char    bidrem6             [  12];    char    _bidrem6             ;    // [long  ,   12] �ż�ȣ���ܷ�6
    char    preoffercha6        [  12];    char    _preoffercha6        ;    // [long  ,   12] �����ŵ�������6
    char    prebidcha6          [  12];    char    _prebidcha6          ;    // [long  ,   12] �����ż�������6
    char    offerho7            [   8];    char    _offerho7            ;    // [long  ,    8] �ŵ�ȣ��7
    char    bidho7              [   8];    char    _bidho7              ;    // [long  ,    8] �ż�ȣ��7
    char    offerrem7           [  12];    char    _offerrem7           ;    // [long  ,   12] �ŵ�ȣ���ܷ�7
    char    bidrem7             [  12];    char    _bidrem7             ;    // [long  ,   12] �ż�ȣ���ܷ�7
    char    preoffercha7        [  12];    char    _preoffercha7        ;    // [long  ,   12] �����ŵ�������7
    char    prebidcha7          [  12];    char    _prebidcha7          ;    // [long  ,   12] �����ż�������7
    char    offerho8            [   8];    char    _offerho8            ;    // [long  ,    8] �ŵ�ȣ��8
    char    bidho8              [   8];    char    _bidho8              ;    // [long  ,    8] �ż�ȣ��8
    char    offerrem8           [  12];    char    _offerrem8           ;    // [long  ,   12] �ŵ�ȣ���ܷ�8
    char    bidrem8             [  12];    char    _bidrem8             ;    // [long  ,   12] �ż�ȣ���ܷ�8
    char    preoffercha8        [  12];    char    _preoffercha8        ;    // [long  ,   12] �����ŵ�������8
    char    prebidcha8          [  12];    char    _prebidcha8          ;    // [long  ,   12] �����ż�������8
    char    offerho9            [   8];    char    _offerho9            ;    // [long  ,    8] �ŵ�ȣ��9
    char    bidho9              [   8];    char    _bidho9              ;    // [long  ,    8] �ż�ȣ��9
    char    offerrem9           [  12];    char    _offerrem9           ;    // [long  ,   12] �ŵ�ȣ���ܷ�9
    char    bidrem9             [  12];    char    _bidrem9             ;    // [long  ,   12] �ż�ȣ���ܷ�9
    char    preoffercha9        [  12];    char    _preoffercha9        ;    // [long  ,   12] �����ŵ�������9
    char    prebidcha9          [  12];    char    _prebidcha9          ;    // [long  ,   12] �����ż�������9
    char    offerho10           [   8];    char    _offerho10           ;    // [long  ,    8] �ŵ�ȣ��10
    char    bidho10             [   8];    char    _bidho10             ;    // [long  ,    8] �ż�ȣ��10
    char    offerrem10          [  12];    char    _offerrem10          ;    // [long  ,   12] �ŵ�ȣ���ܷ�10
    char    bidrem10            [  12];    char    _bidrem10            ;    // [long  ,   12] �ż�ȣ���ܷ�10
    char    preoffercha10       [  12];    char    _preoffercha10       ;    // [long  ,   12] �����ŵ�������10
    char    prebidcha10         [  12];    char    _prebidcha10         ;    // [long  ,   12] �����ż�������10
    char    totofferrem         [  12];    char    _totofferrem         ;    // [long  ,   12] �ŵ�ȣ���Ѽ���
    char    totbidrem           [  12];    char    _totbidrem           ;    // [long  ,   12] �ż�ȣ���Ѽ���
    char    preoffercha         [  12];    char    _preoffercha         ;    // [long  ,   12] �����ŵ�ȣ���Ѵ�����
    char    prebidcha           [  12];    char    _prebidcha           ;    // [long  ,   12] �����ż�ȣ���Ѵ�����
    char    donsigubun          [   1];    char    _donsigubun          ;    // [long  ,    1] ����ȣ������
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�
} KXH1_OutBlock, *LPKXH1_OutBlock;
#define NAME_KXH1_OutBlock     "OutBlock"

class CKBREAL_KXH1
{
public: //Variable
	CString		m_szkshcode;		/*  [�����ڵ�]				*/
	CString		m_szofferho1;		/*  [�ŵ�ȣ�� 1]			*/
	CString		m_szbidho1;			/*  [�ż�ȣ�� 1]			*/
	CString		m_szofferrem1;		/*  [�ŵ�ȣ�� �ܷ� 1]		*/
	CString		m_szbidrem1;		/*  [�ż�ȣ�� �ܷ� 1]		*/
	CString		m_szofferho2;		/*  [�ŵ�ȣ�� 2]			*/
	CString		m_szbidho2;			/*  [�ż�ȣ�� 2]			*/
	CString		m_szofferrem2;		/*  [�ŵ�ȣ�� �ܷ� 2]		*/
	CString		m_szbidrem2;		/*  [�ż�ȣ�� �ܷ� 2]		*/
	CString		m_szofferho3;		/*  [�ŵ�ȣ�� 3]			*/
	CString		m_szbidho3;			/*  [�ż�ȣ�� 3]			*/
	CString		m_szofferrem3;		/*  [�ŵ�ȣ�� �ܷ� 3]		*/
	CString		m_szbidrem3;		/*  [�ż�ȣ�� �ܷ� 3]		*/
	CString		m_szofferho4;		/*  [�ŵ�ȣ�� 4]			*/
	CString		m_szbidho4;			/*  [�ż�ȣ�� 4]			*/
	CString		m_szofferrem4;		/*  [�ŵ�ȣ�� �ܷ� 4]		*/
	CString		m_szbidrem4;		/*  [�ż�ȣ�� �ܷ� 4]		*/
	CString		m_szofferho5;		/*  [�ŵ�ȣ�� 5]			*/
	CString		m_szbidho5;			/*  [�ż�ȣ�� 5]			*/
	CString		m_szofferrem5;		/*  [�ŵ�ȣ�� �ܷ� 5]		*/
	CString		m_szbidrem5;		/*  [�ż�ȣ�� �ܷ� 5]		*/
	CString		m_sztotofferrem;	/*  [�ŵ�ȣ���Ѽ���]		*/
	CString		m_sztotbidrem;		/*  [�ż�ȣ���Ѽ���]		*/

public:
	CKBREAL_KXH1()
	{
	}

	virtual void  Convert(LPKXH1_OutBlock pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, kshcode);		/*  [�����ڵ�]				*/
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/*  [�ŵ�ȣ�� 1]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/*  [�ż�ȣ�� 1]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem1);		/*  [�ŵ�ȣ�� �ܷ� 1]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem1);		/*  [�ż�ȣ�� �ܷ� 1]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/*  [�ŵ�ȣ�� 2]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho2);			/*  [�ż�ȣ�� 2]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem2);		/*  [�ŵ�ȣ�� �ܷ� 2]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem2);		/*  [�ż�ȣ�� �ܷ� 2]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/*  [�ŵ�ȣ�� 3]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho3);			/*  [�ż�ȣ�� 3]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem3);		/*  [�ŵ�ȣ�� �ܷ� 3]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem3);		/*  [�ż�ȣ�� �ܷ� 3]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho4);		/*  [�ŵ�ȣ�� 4]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho4);			/*  [�ż�ȣ�� 4]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem4);		/*  [�ŵ�ȣ�� �ܷ� 4]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem4);		/*  [�ż�ȣ�� �ܷ� 4]		*/
		DATA_MEMCOPY(sTmp, pInput, offerho5);		/*  [�ŵ�ȣ�� 5]			*/
		DATA_MEMCOPY(sTmp, pInput, bidho5);			/*  [�ż�ȣ�� 5]			*/
		DATA_MEMCOPY(sTmp, pInput, offerrem5);		/*  [�ŵ�ȣ�� �ܷ� 5]		*/
		DATA_MEMCOPY(sTmp, pInput, bidrem5);		/*  [�ż�ȣ�� �ܷ� 5]		*/
		DATA_MEMCOPY(sTmp, pInput, totofferrem);	/*  [�ŵ�ȣ���Ѽ���]		*/
		DATA_MEMCOPY(sTmp, pInput, totbidrem);		/*  [�ż�ȣ���Ѽ���]		*/
	}
};

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_CHART_TR_ALL_256A215D_A45E12R__98B1A__INCLUDED_
