#pragma once

/*******************************************************************************
 * �ֽ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/
struct  s_mid   {
	char	accn[11];
	char	pswd[8];
	char	cod2[12];
	char	sygb[1];	// 0.�ſ����� 1.All
	char	fees[10];
};

struct  s_rec   {
	char	cod2[12];	// �����ڵ�
	char	hnam[40];	// �����
	char	jgyh[2];	// �ܰ�����('00'����, '01'�ſ�, '02'~'05'���, '06'��Ÿ)
	char	jqty[10];	// �ܰ����
	char	xqty[10];	// �ŵ�����
	char	pamt[10];	// ������հ�
	char	mamt[15];	// ���Աݾ�

	char	curr[10];	// ���簡
	char    diff[10];   /* ���ϴ��         */
	char    rate[10];   /* �����           */
	char	camt[15];	// �򰡱ݾ�
	char	tamt[15];	// �򰡼���
	char	srat[10];	// �򰡼��ͷ�
	char	sycd[2];	// �ſ��ڵ�
	char	sydt[8];	// ������
	char	samt[15];	// �ſ�ݾ�
};

struct  s_mod   {
	char	accn[11];
	char    nrec[4];
	struct  s_rec   rec[1];
}; 

/*******************************************************************************
 * ����/�ɼ� �ܰ��� ��ȸ IO FORMAT
 ******************************************************************************/

#define mkty_STOCK  '1'
#define mkty_FUTURE '2'
#define mkty_ETC    '3'
#define odty_QUERY  '1'
#define odty_ORDER  '2'
#define odty_TRANS  '3' 

struct f_mid {  // ���� Input
	char accn[11];
	char pswd[8];
	char date[8];
	char cod2[8];
};

struct f_rec {
	char cod2[8];	// �����ڵ�
	char hnam[30];	// �����
	char dsgb[6];	// �Ÿű��� - "+�ż�", "-�ŵ�"

	char bqty[10];	// �ܰ����
	char cqty[10];	// û�갡�ɼ���
	char pprc[10];	// ��հ�
	char curr[10];	// ���簡
	char diff[10];	// ��հ� ���
	char pamt[15];	// �򰡱ݾ�
	char psam[15];	// �򰡼���
	char prat[10];	// ���ͷ�
	char tamt[15];	// ���Աݾ�
};

struct f_mod {
	char accn[11];
	char nrec[4];
	struct f_rec rec[1];
};



#define L_smid sizeof(struct s_mid)    // Length of stock input
#define L_smod sizeof(struct s_mod) - sizeof(struct s_rec) // Length of stock output
#define L_srec sizeof(struct s_rec)    // Length of stock Grid

#define L_fmid sizeof(struct f_mid)    // Length of Future input
#define L_fmod sizeof(struct f_mod) - sizeof(struct f_rec) // Length of Future output
#define L_frec sizeof(struct f_rec)    // Length of Future Grid


