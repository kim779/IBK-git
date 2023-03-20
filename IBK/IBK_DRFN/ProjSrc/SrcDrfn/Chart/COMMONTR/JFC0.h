///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽļ��� ü��(JFC0) ( JFC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_JFC0     "JFC0"

// �Է�
typedef struct _JFC0_InBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY���������ڵ�
} JFC0_InBlock, *LPJFC0_InBlock;
#define NAME_JFC0_InBlock     "InBlock"

// ���
typedef struct _JFC0_OutBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY���������ڵ�
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] ü��ð�
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�
    char    change              [   7];    char    _change              ;    // [long  ,    7] ���ϴ��
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����
    char    price               [   7];    char    _price               ;    // [long  ,    7] ���簡
    char    open                [   7];    char    _open                ;    // [long  ,    7] �ð�
    char    high                [   7];    char    _high                ;    // [long  ,    7] ��
    char    low                 [   7];    char    _low                 ;    // [long  ,    7] ����
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��('+':�ż� '-':�ŵ�)
    char    cvolume             [   7];    char    _cvolume             ;    // [long  ,    7] ü�ᷮ
    char    volume              [   7];    char    _volume              ;    // [long  ,    7] �����ŷ���
    char    volchange           [   7];    char    _volchange           ;    // [long  ,    7] �ŷ������ϴ��
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] �ŷ��������
    char    value               [  15];    char    _value               ;    // [float , 15.2] �����ŷ����
    char    mdvolume            [   7];    char    _mdvolume            ;    // [long  ,    7] �ŵ�����ü�ᷮ
    char    mdchecnt            [   7];    char    _mdchecnt            ;    // [long  ,    7] �ŵ�����ü��Ǽ�
    char    msvolume            [   7];    char    _msvolume            ;    // [long  ,    7] �ż�����ü�ᷮ
    char    mschecnt            [   7];    char    _mschecnt            ;    // [long  ,    7] �ż�����ü��Ǽ�
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] ü�ᰭ��
    char    offerho1            [   7];    char    _offerho1            ;    // [long  ,    7] �ŵ�ȣ��1
    char    bidho1              [   7];    char    _bidho1              ;    // [long  ,    7] �ż�ȣ��1
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] �̰�����������
    char    openyakcha          [   8];    char    _openyakcha          ;    // [long  ,    8] �̰��������������
    char    openyakchange       [   8];    char    _openyakchange       ;    // [long  ,    8] �̰����������ϴ��
    char    baseprice           [   7];    char    _baseprice           ;    // [long  ,    7] �����ڻ갡��
    char    theoryprice         [   7];    char    _theoryprice         ;    // [long  ,    7] �̷а�
    char    grate               [   7];    char    _grate               ;    // [long  ,    7] ������
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] ������
    char    sbasis              [   7];    char    _sbasis              ;    // [long  ,    7] ����BASIS
    char    ibasis              [   7];    char    _ibasis              ;    // [long  ,    7] �̷�BASIS
    char    futcode             [   8];    char    _futcode             ;    // [string,    8] ���������ڵ�
} JFC0_OutBlock, *LPJFC0_OutBlock;
#define NAME_JFC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
