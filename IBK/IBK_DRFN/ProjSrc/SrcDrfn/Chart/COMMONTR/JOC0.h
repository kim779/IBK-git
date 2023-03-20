///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽĿɼ� ü��(JOC0) ( JOC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_JOC0     "JOC0"

// �Է�
typedef struct _JOC0_InBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY�ɼǴ����ڵ�
} JOC0_InBlock, *LPJOC0_InBlock;
#define NAME_JOC0_InBlock     "InBlock"

// ���
typedef struct _JOC0_OutBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY�ɼǴ����ڵ�
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] ü��ð�
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�
    char    change              [   7];    char    _change              ;    // [long  ,    7] ���ϴ��
    char    drate               [   7];    char    _drate               ;    // [float ,  7.2] �����
    char    price               [   7];    char    _price               ;    // [long  ,    7] ���簡
    char    open                [   7];    char    _open                ;    // [long  ,    7] �ð�
    char    high                [   7];    char    _high                ;    // [long  ,    7] ��
    char    low                 [   7];    char    _low                 ;    // [long  ,    7] ����
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��('+':�ż� '-':�ŵ�)
    char    cvolume             [   7];    char    _cvolume             ;    // [string,    7] ü�ᷮ
    char    volume              [   7];    char    _volume              ;    // [string,    7] �����ŷ���
    char    volchange           [   7];    char    _volchange           ;    // [string,    7] �ŷ������ϴ��
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] �ŷ��������
    char    value               [  15];    char    _value               ;    // [float , 15.2] �����ŷ����
    char    mdvolume            [   7];    char    _mdvolume            ;    // [float ,  7.2] �ŵ�����ü�ᷮ
    char    mdchecnt            [   7];    char    _mdchecnt            ;    // [long  ,    7] �ŵ�����ü��Ǽ�
    char    msvolume            [   7];    char    _msvolume            ;    // [float ,    7] �ż�����ü�ᷮ
    char    mschecnt            [   7];    char    _mschecnt            ;    // [long  ,    7] �ż�����ü��Ǽ�
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] �̰�����������
    char    openyakchange       [   8];    char    _openyakchange       ;    // [long  ,    8] �̰����������ϴ��
    char    optcode             [   8];    char    _optcode             ;    // [string,    8] �ɼǴ����ڵ�
} JOC0_OutBlock, *LPJOC0_OutBlock;
#define NAME_JOC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
