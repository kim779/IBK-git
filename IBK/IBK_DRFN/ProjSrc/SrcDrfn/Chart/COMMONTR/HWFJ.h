///////////////////////////////////////////////////////////////////////////////////////////////////
// �ؿ����� ( HWFJ,attr,key=10,keycnt=2000,bufcnt=5,group=1 )
#pragma pack( push, 1 )

#define NAME_HWFJ     "HWFJ"

// �Է�
typedef struct _HWFJ_InBlock
{
    char    symbol              [  10];    char    _symbol              ;    // [string,   10] KEY�ɺ��ڵ�
} HWFJ_InBlock, *LPHWFJ_InBlock;
#define NAME_HWFJ_InBlock     "InBlock"

// ���
typedef struct _HWFJ_OutBlock
{
    char    symbol              [  10];    char    _symbol              ;    // [string,   10] KEY�ɺ��ڵ�
    char    date                [   8];    char    _date                ;    // [string,    8] ����
    char    time                [  10];    char    _time                ;    // [string,   10] �ð�
    char    kodate              [   8];    char    _kodate              ;    // [string,    8] �ѱ�����
    char    kotime              [  10];    char    _kotime              ;    // [string,   10] �ѱ��ð�
    char    open                [   8];    char    _open                ;    // [string,    8] �ð�
    char    high                [   8];    char    _high                ;    // [string,    8] ��
    char    low                 [   8];    char    _low                 ;    // [string,    8] ����
    char    price               [   8];    char    _price               ;    // [string,    8] ���簡
    char    sign                [   1];    char    _sign                ;    // [string,    1] ���ϴ�񱸺�
    char    change              [   8];    char    _change              ;    // [string,    8] ���ϴ��
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] �����
    char    bid                 [   8];    char    _bid                 ;    // [string,    8] �ż�ȣ��
    char    bidrem              [   9];    char    _bidrem              ;    // [long  ,    9] �ż��ܷ�
    char    offer               [   8];    char    _offer               ;    // [string,    8] �ŵ�ȣ��
    char    offerrem            [   9];    char    _offerrem            ;    // [long  ,    9] �ŵ��ܷ�
    char    cvolume             [   9];    char    _cvolume             ;    // [long  ,    9] ü�����
    char    volume              [   9];    char    _volume              ;    // [long  ,    9] �ŷ���
} HWFJ_OutBlock, *LPHWFJ_OutBlock;
#define NAME_HWFJ_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
