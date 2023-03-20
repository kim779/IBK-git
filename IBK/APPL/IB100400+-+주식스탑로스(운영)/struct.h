
#pragma once
#pragma pack(1)
/*******************************************************************************
 * �ֽ� �ܰ��� ��ȸ IO FORMAT _______ PIBOSJGO
 ******************************************************************************/
#define	CREDIT	// �ſ���

struct	s_mid	{		// �ֽ� Input

    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.�ſ����� 1.All */
    char    fill[10];
};

struct	s_rec	{

	char	code[12];	// �����ڵ�
	char	hnam[40];	// �����
    char    jgyh[2];    /* �ܰ�����         */
    char    jqty[10];   /* �ܰ����         */
    char    xqty[10];   /* �ŵ�����         */
    char    pamt[10];   /* ������հ�       */
    char    mamt[15];   /* ���Աݾ�         */

    char    curr[10];   /* ���簡     (*)   */
	char    diff[10];   /* ���ϴ��         */
    char    rate[10];   /* �����           */
    char    camt[15];   /* �򰡱ݾ�         */
    char    tamt[15];   /* �򰡼���   (*)   */
    char    srat[10];   /* �򰡼��ͷ� (*)   */
    char    sycd[2];    /* �ſ��ڵ�         */
    char    sydt[8];    /* ������           */
    char    samt[15];   /* �ſ�ݾ�         */
};

struct	s_mod	{
	char	accn[11];
	char	nrec[4];
	struct	s_rec	rec[1];
};

struct	_recR	{
	char	ask1[9];		// 1���ŵ�ȣ��
	char	ask2[9];		// 2���ŵ�ȣ��
	char	bid1[9];		// 1���ż�ȣ��
	char	bid2[9];		// 2���ż�ȣ��
};
#define	L_recR		sizeof(struct _recR)

//��������
struct _config
{
	int istopexe[4]{}; //0-�սǸ�ǥ, 1-����, 2-����, 3-�������
	int iunit[4]{};//0-�սǸ�ǥ, 1-����, 2-����, 3-�������
	int ival[4]{};//0-�սǸ�ǥ, 1-����, 2-����, 3-�������
	
	int igubn{};//����:0-������, 1-���尡, 2-���Ǻ�, 3-������
	int ivoltype{};//0-%, 1-�Է�
	int ivolval{};//��������
	int ivol{};//����
	int iprctype{};//����Ÿ��
	CString sprctype;//����Ÿ�� �̸�
	int iprcval{};//���� 

	int iauto[2]{};
	int	inow{};//����:0-�ƴϿ�,1-��
};

#define	nREC		4
/*******************************************************************************
//	order format
/*******************************************************************************/
//
//	order format
//
struct	_orderR	{
	char	nrec[nREC];//ó������
	char	odgb;//�ֹ����� 1: �Ϲ�(��Ź,����)  2: �����ɼ� 3. ���ָŸ� 4: �ſ�Ÿ� 5: ä�ǸŸ�
	char	mkgb;//���屸�� 1: �ŷ��� 2: �ڽ��� 3: ��������  5: ���
	char    mmgb;//�Ÿű��� 1: �ŵ� 2: �ż� 3: ���� 4: ���
 	char	accn[11];//
	char	pswd[8];
	char	orgn[12];//�ֹ���ȣ
	char	code[12];//�����ڵ�
	char	jmsr[8];//�ֹ�����
	char	jprc[10];//�ֹ��ܰ�
	char	hogb[2];/*ȣ������ ** �ϹݸŸ� 
						00: ������ 03: ���尡 05: ���Ǻ������� 06: ������������ 07: �ֿ켱������
						09: �ڻ��� 10: �ڻ��ֽ���ɼ� 12: �ڻ��ֽ�Ź 
						61: �尳�����ð��� 81: �ð������� 91:�ð��ܴ��ϰ� 
						99: ���ֹ�ȣ��(�����ֹ��� ���)*/
	char	jmgb[2];//�ֹ����� 01: IOC 02: FOK
//	char	jjgb[1];//������� 1: �Ϻ� 2: ����
	char	crgb[1];/* ó������ A, B, C, D (�Ʒ��� 30 Bytes �� �ֹ������� ���� ���������� ���)
					** ������ ���� ���õǾ����� ���� ��� ��X��
					** ������ 30bytes �� �ʿ�� �߰� ���� ����*/
	char	fill[30];
};
#define	L_orderR	sizeof(struct _orderR)

struct	_respR	{
	char	nrec[nREC];		// only "0001"
	char	jmno[6];
	char	jjcs[6];
	char	omsg[80];
};


struct _trkey
{
	UINT	gubn:3;
	UINT	key:7;
};

#pragma pack()