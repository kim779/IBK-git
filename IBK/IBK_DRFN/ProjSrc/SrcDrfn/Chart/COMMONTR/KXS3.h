///////////////////////////////////////////////////////////////////////////////////////////////////
// KRX ü��(KXS3) ( KXS3,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_KXS3     "KXS3"

// �Է�
typedef struct _KXS3_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY�����ڵ�
} KXS3_InBlock, *LPKXS3_InBlock;
#define NAME_KXS3_InBlock     "InBlock"

// ���
typedef struct _KXS3_OutBlock
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
    char    openchange          [   8];    char    _openchange          ;    // [long  ,    8] �ð����ϴ��
    char    opendrate           [   6];    char    _opendrate           ;    // [float ,  6.2] �ð����ϴ������
    char    highchange          [   8];    char    _highchange          ;    // [long  ,    8] �����ϴ��
    char    highdrate           [   6];    char    _highdrate           ;    // [float ,  6.2] �����ϴ������
    char    lowchange           [   8];    char    _lowchange           ;    // [long  ,    8] �������ϴ��
    char    lowdrate            [   6];    char    _lowdrate            ;    // [float ,  6.2] �������ϴ������
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] ü�ᱸ��('+':�ż� '-':�ŵ�)
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] ü�ᷮ
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] �����ŷ���
    char    volchange           [  12];    char    _volchange           ;    // [long  ,   12] �ŷ������ϴ��
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] �ŷ��������
    char    voltrate            [   6];    char    _voltrate            ;    // [float ,  6.2] �ŷ���ȸ����
    char    value               [  15];    char    _value               ;    // [long  ,   15] �����ŷ����
    char    valchange           [  15];    char    _valchange           ;    // [float , 15.2] �ŷ�������ϴ��
    char    valdrate            [  10];    char    _valdrate            ;    // [float , 10.2] �ŷ���ݵ����
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] �ŵ�����ü�ᷮ
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] �ŵ�����ü��Ǽ�
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] �ż�����ü�ᷮ
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] �ż�����ü��Ǽ�
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] ü�ᰭ��
    char    w_avrg              [   8];    char    _w_avrg              ;    // [float ,  8.2] ���� ��հ�
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] �ŵ�ȣ��
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] �ż�ȣ��
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] �ŵ�ȣ���ܷ�1
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] �ż�ȣ���ܷ�1
    char    totofferrem         [  12];    char    _totofferrem         ;    // [long  ,   12] �ŵ�ȣ���Ѽ���
    char    totbidrem           [  12];    char    _totbidrem           ;    // [long  ,   12] �ż�ȣ���Ѽ���
    char    status              [   2];    char    _status              ;    // [long  ,    2] �屸������
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] ���ϵ��ð���ŷ���(30�ʴ���)
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�
} KXS3_OutBlock, *LPKXS3_OutBlock;
#define NAME_KXS3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
