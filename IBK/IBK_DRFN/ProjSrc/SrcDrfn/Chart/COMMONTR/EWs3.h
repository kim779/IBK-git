#if !defined(AFX_OHHTS_ELW_CNTG__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_)
#define AFX_OHHTS_ELW_CNTG__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// ELW ü��(EWs3) ( EWs3,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_EWs3     "EWs3"

// �Է�
typedef struct _EWs3_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
} EWs3_InBlock, *LPEWs3_InBlock;
#define NAME_EWs3_InBlock     "InBlock"

// ���
typedef struct _EWs3_OutBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] ü��ð�
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���ϴ��
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��('+':�ż� '-':�ŵ�)
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�ᷮ
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���
    char    value               [  15];    char    _value               ;    // [long  ,   15] �����ŷ����
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�����ü�ᷮ
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�����ü��Ǽ�
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�����ü�ᷮ
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�����ü��Ǽ�
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] ü�ᰭ��
    char    w_avrg              [   8];    char    _w_avrg              ;    // [long  ,    8] ������հ�
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] �ŵ�ȣ��
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] �ż�ȣ��
    char    status              [   2];    char    _status              ;    // [long  ,    2] �屸������
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϵ��ð���ŷ���
    char    premium             [   8];    char    _premium             ;    // [float ,    8] �����̾�
    char    moneyness           [   1];    char    _moneyness           ;    // [string,    1] ATM����(1:ATM 2:ITM 3:OTM)
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�
} EWs3_OutBlock, *LPEWs3_OutBlock;
#define NAME_EWs3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_OHHTS_ELW_CNTG__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
