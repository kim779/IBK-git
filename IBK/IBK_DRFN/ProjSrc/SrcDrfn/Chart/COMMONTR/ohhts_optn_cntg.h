#if !defined(AFX_OHHTS_OPTN_CNTG__H_A0D8DC83_1BE0_40a1_9798_33283A4E05BF_INCLUDED_)
#define AFX_OHHTS_OPTN_CNTG__H_A0D8DC83_1BE0_40a1_9798_33283A4E05BF_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ɼ� ü��(OOC0) ( OOC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_OOC0     "OOC0"

// �Է�
typedef struct _OOC0_InBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY�ɼǴ����ڵ�
} OOC0_InBlock, *LPOOC0_InBlock;
#define NAME_OOC0_InBlock     "InBlock"

// ���
typedef struct _OOC0_OutBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY�ɼǴ����ڵ�
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] ü��ð�
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�
    char    change              [   6];    char    _change              ;    // [float ,  6.2] ���ϴ��
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����
    char    price               [   6];    char    _price               ;    // [float ,  6.2] ���簡
    char    open                [   6];    char    _open                ;    // [float ,  6.2] �ð�
    char    high                [   6];    char    _high                ;    // [float ,  6.2] ��
    char    low                 [   6];    char    _low                 ;    // [float ,  6.2] ����
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��('+':�ż� '-':�ŵ�)
    char    cvolume             [   6];    char    _cvolume             ;    // [long  ,    6] ü�ᷮ
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���
    char    volchange           [  12];    char    _volchange           ;    // [float , 12.2] �ŷ������ϴ��
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] �ŷ��������
    char    value               [  12];    char    _value               ;    // [long  ,   12] �����ŷ����
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�����ü�ᷮ
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�����ü��Ǽ�
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�����ü�ᷮ
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�����ü��Ǽ�
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] ü�ᰭ��
    char    offerho1            [   6];    char    _offerho1            ;    // [float ,  6.2] �ŵ�ȣ��1
    char    bidho1              [   6];    char    _bidho1              ;    // [float ,  6.2] �ż�ȣ��1
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] �̰�����������
    char    openyakchange       [   8];    char    _openyakchange       ;    // [long  ,    8] �̰����������ϴ��
    char    k200jisu            [   6];    char    _k200jisu            ;    // [float ,  6.2] KOSPI200����
    char    eqva                [   7];    char    _eqva                ;    // [float ,  7.2] KOSPI�
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] �̷а�
    char    grate               [   6];    char    _grate               ;    // [float ,  6.2] ������
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] ������
    char    impv                [   6];    char    _impv                ;    // [float ,  6.2] ���纯����
    char    intvalue            [   6];    char    _intvalue            ;    // [float ,  6.2] ���簡ġ
    char    timevalue           [   6];    char    _timevalue           ;    // [float ,  6.2] �ð���ġ
    char    jgubun              [   2];    char    _jgubun              ;    // [string,    2] ������
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϵ��ð���ŷ���
    char    optcode             [   8];    char    _optcode             ;    // [string,    8] �ɼǴ����ڵ�
} OOC0_OutBlock, *LPOOC0_OutBlock;
#define NAME_OOC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif // AFX_OHHTS_OPTN_CNTG__H_A0D8DC83_1BE0_40a1_9798_33283A4E05BF_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
