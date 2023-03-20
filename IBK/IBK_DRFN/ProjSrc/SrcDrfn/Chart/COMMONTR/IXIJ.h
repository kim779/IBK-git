///////////////////////////////////////////////////////////////////////////////////////////////////
// ���� �ǽð�(IXIJ) ( IXIJ,attr,key=3,group=1 )
#pragma pack( push, 1 )

#define NAME_IXIJ     "IXIJ"

// �Է�
typedef struct _IXIJ_InBlock
{
    char    kupcode             [   3];    char    _kupcode             ;    // [string,    3] KEY�����ڵ�
} IXIJ_InBlock, *LPIXIJ_InBlock;
#define NAME_IXIJ_InBlock     "InBlock"

// ���
typedef struct _IXIJ_OutBlock
{
    char    kupcode             [   3];    char    _kupcode             ;    // [string,    3] KEY�����ڵ�
    char    time                [   6];    char    _time                ;    // [string,    6] �ð�
    char    jisu                [   8];    char    _jisu                ;    // [float ,  8.2] ����
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�
    char    change              [   8];    char    _change              ;    // [float ,  8.2] ���ϴ��
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�ᷮ
    char    volume              [   8];    char    _volume              ;    // [long  ,    8] �ŷ���
    char    volchange           [   8];    char    _volchange           ;    // [long  ,    8] �ŷ������ϴ��
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] �ŷ��������
    char    value               [   8];    char    _value               ;    // [long  ,    8] �ŷ����
    char    upjo                [   4];    char    _upjo                ;    // [long  ,    4] ���������
    char    highjo              [   4];    char    _highjo              ;    // [long  ,    4] ��������
    char    unchgjo             [   4];    char    _unchgjo             ;    // [long  ,    4] ���������
    char    lowjo               [   4];    char    _lowjo               ;    // [long  ,    4] �϶������
    char    downjo              [   4];    char    _downjo              ;    // [long  ,    4] ���������
    char    upjrate             [   6];    char    _upjrate             ;    // [float ,  6.2] ����������
    char    openjisu            [   8];    char    _openjisu            ;    // [float ,  8.2] �ð�����
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] �ð��ð�
    char    highjisu            [   8];    char    _highjisu            ;    // [float ,  8.2] ������
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] ���ð�
    char    lowjisu             [   8];    char    _lowjisu             ;    // [float ,  8.2] ��������
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] �����ð�
    char    frgsvolume          [   8];    char    _frgsvolume          ;    // [long  ,    8] ���μ��ż�����
    char    orgsvolume          [   8];    char    _orgsvolume          ;    // [long  ,    8] ������ż�����
    char    frgsvalue           [  10];    char    _frgsvalue           ;    // [long  ,   10] ���μ��ż��ݾ�
    char    orgsvalue           [  10];    char    _orgsvalue           ;    // [long  ,   10] ������ż��ݾ�
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] �����ڵ�
} IXIJ_OutBlock, *LPIXIJ_OutBlock;
#define NAME_IXIJ_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
