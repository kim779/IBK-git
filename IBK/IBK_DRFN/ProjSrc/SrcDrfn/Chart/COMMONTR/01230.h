///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ�ȣ��5����ȸ(01230) ( 01230,block,attr,headtype=A )
#pragma pack( push, 1 )

#define NAME_01230     "01230"

// �⺻�Է�
typedef struct _k01230In1
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] �����ڵ�
} k01230In1, *LPk01230In1;
#define NAME_k01230In1     "k01230In1"

// ��� List
typedef struct _k01230Out1
{
    char    hname               [  20];    char    _hname               ;    // [string,   20] �����
    char    price               [   8];    char    _price               ;    // [long  ,    8] ���簡
    char    sign                [   1];    char    _sign                ;    // [string,    1] ��񱸺�
    char    change              [   8];    char    _change              ;    // [long  ,    8] ���
    char    rate                [   7];    char    _rate                ;    // [float ,  7.2] �����
    char    volume              [  12];    char    _volume              ;    // [float ,   12] �ŷ���
    char    volrate             [   7];    char    _volrate             ;    // [float ,  7.2] ���ϰŷ������
    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] �켱�ŵ�ȣ��
    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] �켱�ż�ȣ��
    char    fholdvol            [  12];    char    _fholdvol            ;    // [float ,   12] �ܱ��κ�������
    char    exhratio            [   6];    char    _exhratio            ;    // [float ,  6.2] �ܱ��κ�������
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] ȣ�����Žð�
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] �ŵ�ȣ��1
    char    offerrem1           [   8];    char    _offerrem1           ;    // [long  ,    8] �ŵ�ȣ������1
    char    preoffercha1        [   8];    char    _preoffercha1        ;    // [long  ,    8] �ŵ�����������1
    char    offerho2            [   8];    char    _offerho2            ;    // [long  ,    8] �ŵ�ȣ��2
    char    offerrem2           [   8];    char    _offerrem2           ;    // [long  ,    8] �ŵ�ȣ������2
    char    preoffercha2        [   8];    char    _preoffercha2        ;    // [long  ,    8] �ŵ�����������2
    char    offerho3            [   8];    char    _offerho3            ;    // [long  ,    8] �ŵ�ȣ��3
    char    offerrem3           [   8];    char    _offerrem3           ;    // [long  ,    8] �ŵ�ȣ������3
    char    preoffercha3        [   8];    char    _preoffercha3        ;    // [long  ,    8] �ŵ�����������3
    char    offerho4            [   8];    char    _offerho4            ;    // [long  ,    8] �ŵ�ȣ��4
    char    offerrem4           [   8];    char    _offerrem4           ;    // [long  ,    8] �ŵ�ȣ������4
    char    preoffercha4        [   8];    char    _preoffercha4        ;    // [long  ,    8] �ŵ�����������4
    char    offerho5            [   8];    char    _offerho5            ;    // [long  ,    8] �ŵ�ȣ��5
    char    offerrem5           [   8];    char    _offerrem5           ;    // [long  ,    8] �ŵ�ȣ������5
    char    preoffercha5        [   8];    char    _preoffercha5        ;    // [long  ,    8] �ŵ�����������5
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] �ż�ȣ��1
    char    bidrem1             [   8];    char    _bidrem1             ;    // [long  ,    8] �ż�ȣ������1
    char    prebidcha1          [   8];    char    _prebidcha1          ;    // [long  ,    8] �ż�����������1
    char    bidho2              [   8];    char    _bidho2              ;    // [long  ,    8] �ż�ȣ��2
    char    bidrem2             [   8];    char    _bidrem2             ;    // [long  ,    8] �ż�ȣ������2
    char    prebidcha2          [   8];    char    _prebidcha2          ;    // [long  ,    8] �ż�����������2
    char    bidho3              [   8];    char    _bidho3              ;    // [long  ,    8] �ż�ȣ��3
    char    bidrem3             [   8];    char    _bidrem3             ;    // [long  ,    8] �ż�ȣ������3
    char    prebidcha3          [   8];    char    _prebidcha3          ;    // [long  ,    8] �ż�����������3
    char    bidho4              [   8];    char    _bidho4              ;    // [long  ,    8] �ż�ȣ��4
    char    bidrem4             [   8];    char    _bidrem4             ;    // [long  ,    8] �ż�ȣ������4
    char    prebidcha4          [   8];    char    _prebidcha4          ;    // [long  ,    8] �ż�����������4
    char    bidho5              [   8];    char    _bidho5              ;    // [long  ,    8] �ż�ȣ��5
    char    bidrem5             [   8];    char    _bidrem5             ;    // [long  ,    8] �ż�ȣ������5
    char    prebidcha5          [   8];    char    _prebidcha5          ;    // [long  ,    8] �ż�����������5
    char    totofferrem         [   8];    char    _totofferrem         ;    // [long  ,    8] �ŵ�ȣ���Ѽ���
    char    preoffercha         [   8];    char    _preoffercha         ;    // [long  ,    8] �ŵ�ȣ���Ѽ����������
    char    totbidrem           [   8];    char    _totbidrem           ;    // [long  ,    8] �ż�ȣ���Ѽ���
    char    prebidcha           [   8];    char    _prebidcha           ;    // [long  ,    8] �ż�ȣ���Ѽ����������
    char    yeprice             [   8];    char    _yeprice             ;    // [long  ,    8] ����ü�ᰡ��
    char    yevolume            [   8];    char    _yevolume            ;    // [long  ,    8] ����ü�����
    char    jnilysign           [   1];    char    _jnilysign           ;    // [string,    1] ����ü�ᰡ����������񱸺�
    char    jnilychange         [   8];    char    _jnilychange         ;    // [long  ,    8] ����ü�ᰡ�����������
    char    yerate              [   6];    char    _yerate              ;    // [float ,  6.2] ��������
    char    tmofferrem          [   8];    char    _tmofferrem          ;    // [long  ,    8] �ð��ܸŵ��ܷ�
    char    tmbidrem            [   8];    char    _tmbidrem            ;    // [long  ,    8] �ð��ܸż��ܷ�
    char    ho_status           [   1];    char    _ho_status           ;    // [long  ,    1] �屸��
    char    recprice            [   8];    char    _recprice            ;    // [long  ,    8] ���ذ�
    char    uplmtprice          [   8];    char    _uplmtprice          ;    // [long  ,    8] ���Ѱ�
    char    dnlmtprice          [   8];    char    _dnlmtprice          ;    // [long  ,    8] ���Ѱ�
    char    open                [   8];    char    _open                ;    // [long  ,    8] �ð�
    char    high                [   8];    char    _high                ;    // [long  ,    8] ��
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] ����
    char    trade_cost          [   8];    char    _trade_cost          ;    // [long  ,    8] �ŷ����
    char    volro               [   9];    char    _volro               ;    // [float ,  9.2] �ŷ���ȸ����
} k01230Out1, *LPk01230Out1;
#define NAME_k01230Out1     "k01230Out1"

typedef struct _k01230Out1_Ex
{
	char    price               [   8];
	char    uplmtprice          [   8];
	char    dnlmtprice          [   8];
	char    offerho3            [   8];
	char    offerho2            [   8];
	char    offerho1            [   8];
	char    bidho1              [   8];
	char    bidho2              [   8];
	char    bidho3              [   8];
} k01230Out1_Ex, *LPk01230Out1_Ex;

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
