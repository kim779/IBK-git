///////////////////////////////////////////////////////////////////////////////////////////////////
// ����˻� �ý��� ���� ��ȣ(FNDA) ( FNDA,attr,key=4,group=1 )
#pragma pack( push, 1 )

#define NAME_FNDA     "FNDA"

// �Է�
typedef struct _FNDA_InBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] KEY��Ŷ�����ڵ�
} FNDA_InBlock, *LPFNDA_InBlock;
#define NAME_FNDA_InBlock     "InBlock"

// ���
typedef struct _FNDA_OutBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] KEY��Ŷ�����ڵ�
    char    gsalertgubun        [   1];    char    _gsalertgubun        ;    // [string,    1] System('S') User('U')
    char    gsdate              [   8];    char    _gsdate              ;    // [string,    8] ��ȣ�߻�����
    char    gstime              [   6];    char    _gstime              ;    // [string,    6] ��ȣ�߻��ð�
    char    gstruecnt           [   4];    char    _gstruecnt           ;    // [string,    4] ���縸�� �����(����+�ű�)
    char    gsdatacnt           [   4];    char    _gsdatacnt           ;    // [long  ,    4] ���������
    char    gsstatus            [   1];    char    _gsstatus            ;    // [string,    1] ��ȣ����
    char    gsjobflag           [   1];    char    _gsjobflag           ;    // [string,    1] �ű�:'N' ������:'R' Ż��:'O'
    char    gscode              [   6];    char    _gscode              ;    // [string,    6] �����ڵ�
    char    gshname             [  20];    char    _gshname             ;    // [string,   20] �����
    char    gsentrycnt          [   4];    char    _gsentrycnt          ;    // [long  ,    4] ����Ƚ��
    char    gsmanagement        [   1];    char    _gsmanagement        ;    // [string,    1] ��������
    char    gscontrol           [   1];    char    _gscontrol           ;    // [string,    1] �̻�޵�����
    char    gstradestop         [   1];    char    _gstradestop         ;    // [string,    1] �ŷ���������
    char    gsinvestattention   [   1];    char    _gsinvestattention   ;    // [string,    1] ������������
    char    gsinsinceritynotice [   1];    char    _gsinsinceritynotice ;    // [string,    1] �Ҽ��ǰ�������
    char    gsconfidencelimit   [   1];    char    _gsconfidencelimit   ;    // [string,    1] �ſ���������
    char    gscodenotice        [   1];    char    _gscodenotice        ;    // [string,    1] �������
    char    gsjungrimeme        [   1];    char    _gsjungrimeme        ;    // [string,    1] �����Ÿ�����
    char    gsmarket            [   1];    char    _gsmarket            ;    // [string,    1] ���屸��
    char    gsdepositmoney      [   3];    char    _gsdepositmoney      ;    // [long  ,    3] ���űݺ���
    char    gsprice             [   8];    char    _gsprice             ;    // [long  ,    8] ���簡
    char    gssign              [   1];    char    _gssign              ;    // [string,    1] ���ϴ������ȣ
    char    gschange            [   8];    char    _gschange            ;    // [long  ,    8] ���ϴ��
    char    gschgrate           [   6];    char    _gschgrate           ;    // [float ,  6.2] ���ϴ������
    char    gsvolume            [   9];    char    _gsvolume            ;    // [long  ,    9] �ŷ���
    char    gsvolumerate        [   6];    char    _gsvolumerate        ;    // [float ,  6.2] �ŷ������ϴ����
    char    gsamount            [   9];    char    _gsamount            ;    // [long  ,    9] �ŷ����
} FNDA_OutBlock, *LPFNDA_OutBlock;
#define NAME_FNDA_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////