///////////////////////////////////////////////////////////////////////////////////////////////////
// ����˻� ����ü�ᷮ(CVOL) ( CVOL,attr,key=4,group=1 )
#pragma pack( push, 1 )

#define NAME_CVOL     "CVOL"

// �Է�
typedef struct _CVOL_InBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] ��Ŷ�����ڵ�
} CVOL_InBlock, *LPCVOL_InBlock;
#define NAME_CVOL_InBlock     "InBlock"

// ���
typedef struct _CVOL_OutBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] ��Ŷ�����ڵ�
    char    gsalertgubun        [   1];    char    _gsalertgubun        ;    // [string,    1] System('S') User('U')
    char    gsdate              [   8];    char    _gsdate              ;    // [string,    8] ��ȣ�߻�����
    char    gstime              [   6];    char    _gstime              ;    // [string,    6] ��ȣ�߻��ð�
    char    gstruecnt           [   4];    char    _gstruecnt           ;    // [long  ,    4] ���縸�������(����+�ű�)
    char    gsdatacnt           [   4];    char    _gsdatacnt           ;    // [long  ,    4] ���������
    char    gsstatus            [   1];    char    _gsstatus            ;    // [string,    1] ��ȣ�߻�:'S' �����̻�:'E'
    char    sjobflag            [   1];    char    _sjobflag            ;    // [string,    1] �ű�:'N' ������:'R' Ż��:'O'
    char    scode               [   6];    char    _scode               ;    // [string,    6] �����ڵ�
    char    shname              [  20];    char    _shname              ;    // [string,   20] �����
    char    sentrycnt           [   4];    char    _sentrycnt           ;    // [long  ,    4] ����Ƚ��
    char    sup_l               [   2];    char    _sup_l               ;    // [string,    2] ������з�(��2:��3:��3)
    char    sup_m               [   3];    char    _sup_m               ;    // [string,    3] �����ߺз�
    char    sup_s               [   3];    char    _sup_s               ;    // [string,    3] �����Һз�
    char    sprefershare        [   1];    char    _sprefershare        ;    // [string,    1] �켱��
    char    smanagement         [   1];    char    _smanagement         ;    // [string,    1] ��������
    char    scontrol            [   1];    char    _scontrol            ;    // [string,    1] �̻�޵�����
    char    stradestop          [   1];    char    _stradestop          ;    // [string,    1] �ŷ���������
    char    sinsinceritynotice  [   1];    char    _sinsinceritynotice  ;    // [string,    1] �Ҽ��ǰ�������
    char    sconfidencelimit    [   1];    char    _sconfidencelimit    ;    // [string,    1] �ſ���������
    char    scodenotice         [   1];    char    _scodenotice         ;    // [string,    1] �������
    char    sjungrimeme         [   1];    char    _sjungrimeme         ;    // [string,    1] �����Ÿ�����
    char    smarket             [   1];    char    _smarket             ;    // [string,    1] ���屸��
    char    sdepositmoney       [   3];    char    _sdepositmoney       ;    // [long  ,    3] ���űݺ���
    char    sinvestrisk         [   1];    char    _sinvestrisk         ;    // [string,    1] ������������
    char    sinvestwarn         [   1];    char    _sinvestwarn         ;    // [string,    1] ���ڰ������
    char    sinvestattention    [   1];    char    _sinvestattention    ;    // [string,    1] ������������
    char    sinvestrisknotice   [   1];    char    _sinvestrisknotice   ;    // [string,    1] �������迹������
    char    sprice              [   8];    char    _sprice              ;    // [long  ,    8] ���簡
    char    ssign               [   1];    char    _ssign               ;    // [string,    1] ���ϴ������ȣ
    char    schange             [   8];    char    _schange             ;    // [long  ,    8] ���ϴ��
    char    schgrate            [   6];    char    _schgrate            ;    // [float ,  6.2] ���ϴ������
    char    svolume             [   9];    char    _svolume             ;    // [long  ,    9] �ŷ���
    char    svolumerate         [   6];    char    _svolumerate         ;    // [float ,  6.2] �ŷ������ϴ����
    char    samount             [   9];    char    _samount             ;    // [long  ,    9] �ŷ����
    char    scvolume            [   9];    char    _scvolume            ;    // [long  ,    9] ����ü�ᷮ
    char    scvolumerate        [   6];    char    _scvolumerate        ;    // [long  ,    6] �����ŷ���������ü�ᷮ
    char    scamount            [   9];    char    _scamount            ;    // [long  ,    9] ����ü����
    char    scgubun             [   1];    char    _scgubun             ;    // [string,    1] ü�ᱸ��('+':�ż� '-':�ŵ�)
} CVOL_OutBlock, *LPCVOL_OutBlock;
#define NAME_CVOL_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
