#if !defined(AFX_NAME_KXYS_256A215D_A45E12R__98B1A__INCLUDED_)
#define AFX_NAME_KXYS_256A215D_A45E12R__98B1A__INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
// KRX ����ü��(KXYS) ( KXYS,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_KXYS     "KXYS"

// �Է�
typedef struct _KXYS_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
} KXYS_InBlock, *LPKXYS_InBlock;
#define NAME_KXYS_InBlock     "InBlock"

// ���
typedef struct _KXYS_OutBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ���ð�
    char    yeprice             [   8];    char    _yeprice             ;    // [long  ,    8] ����ü�ᰡ��
    char    preysign            [   1];    char    _preysign            ;    // [string,    1] ����ü�ᰡ�������簡��񱸺�
    char    preychange          [   8];    char    _preychange          ;    // [long  ,    8] ����ü�ᰡ�������簡���
    char    preydrate           [   6];    char    _preydrate           ;    // [float ,  6.2] ����ü�ᰡ�������簡�����
    char    jnilysign           [   1];    char    _jnilysign           ;    // [string,    1] ����ü�ᰡ����������񱸺�
    char    jnilychange         [   8];    char    _jnilychange         ;    // [long  ,    8] ����ü�ᰡ�����������
    char    jnilydrate          [   6];    char    _jnilydrate          ;    // [float ,  6.2] ����ü�ᰡ�������������
    char    yecvolume           [   8];    char    _yecvolume           ;    // [long  ,    8] ����ü�ắ����
    char    yevolume            [  12];    char    _yevolume            ;    // [long  ,   12] ����ü�����
    char    yvolchange          [  12];    char    _yvolchange          ;    // [long  ,   12] ����ü��������ϰŷ������
    char    yofferho0           [   8];    char    _yofferho0           ;    // [long  ,    8] ����ŵ�ȣ��
    char    ybidho0             [   8];    char    _ybidho0             ;    // [long  ,    8] ����ż�ȣ��
    char    yofferrem0          [  12];    char    _yofferrem0          ;    // [long  ,   12] ����ŵ�ȣ������
    char    ybidrem0            [  12];    char    _ybidrem0            ;    // [long  ,   12] ����ż�ȣ������
    char    ypreoffercha0       [  12];    char    _ypreoffercha0       ;    // [long  ,   12] ���������ŵ�������
    char    yprebidcha0         [  12];    char    _yprebidcha0         ;    // [long  ,   12] ���������ż�������
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�
} KXYS_OutBlock, *LPKXYS_OutBlock;
#define NAME_KXYS_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_NAME_KXYS_256A215D_A45E12R__98B1A__INCLUDED_
