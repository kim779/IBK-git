#if !defined(AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_)
#define AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ü��(FFC0) ( FFC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_FFC0     "FFC0"

// �Է�
typedef struct _FFC0_InBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY���������ڵ�
} FFC0_InBlock, *LPFFC0_InBlock;
#define NAME_FFC0_InBlock     "InBlock"

// ���
typedef struct _FFC0_OutBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY���������ڵ�
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
    char    volchange           [  12];    char    _volchange           ;    // [long  ,   12] �ŷ������ϴ��
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
    char    k200jisu            [   6];    char    _k200jisu            ;    // [float ,  6.2] KOSPI200 ����
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] �̷а�
    char    grate               [   6];    char    _grate               ;    // [float ,  6.2] ������
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] ������
    char    sbasis              [   6];    char    _sbasis              ;    // [float ,  6.2] ����BASIS
    char    ibasis              [   6];    char    _ibasis              ;    // [float ,  6.2] �̷�BASIS
    char    jgubun              [   2];    char    _jgubun              ;    // [string,    2] ������
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϵ��ð��� �ŷ���
    char    futcode             [   8];    char    _futcode             ;    // [string,    8] �����ڵ�
} FFC0_OutBlock, *LPFFC0_OutBlock;
#define NAME_FFC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif // AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
