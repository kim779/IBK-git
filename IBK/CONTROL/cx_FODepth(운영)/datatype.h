
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2001
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : datatype.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-07	Initial version
// *****************************************************************************

#ifndef _DATATYPE_
#define _DATATYPE_

#define	WM_SCRL		WM_USER+100	// scroll message by Contract
#define	SCRL_VCHANGE	1		// change vscroll by Contract
#define	WM_MBONG	WM_USER+200	// MBong message by lbuttondown

struct	_price {
	char	ask[8];			// �ŵ�ȣ��
	char	bid[8];			// �ż�ȣ��
	char	avol[9];		// �ŵ�ȣ������
	char	bvol[9];		// �ż�ȣ������
	char	acha[9];		// �����ŵ�������
	char	bcha[9];		// �����ż�������
};


struct	_hoga {
	char	codx[12];		// CODX
	char	curr[9];		// ���簡
	char	pcls[8];		// ��������
	char	gvol[12];		// �ŷ���
	char	gamt[12];		// �ŷ�����
        char    siga[8];                // �ð�
        char    koga[8];                // ��
        char    jega[8];                // ����
        char    amga[8];                // �׸鰡
	char	ysga[8];		// ����ü�ᰡ / ���� ����
	char	yvol[9];		// ����ü����� / ���� �ŷ���
	char	htim[8];		// ȣ���ð�
	char    gjga[8];                // ���ذ�
        char    pgga[8];                // ������հ�
        char    sjrt[6];                // ������       999V99
        char    shga[8];                // ���Ѱ�
        char    hhga[8];                // ���Ѱ�
	struct	_price	price[10];	// 10ȣ�� 0:�켱ȣ�� 1,2,:��,��
	char	avols[9];		// �ŵ�ȣ���Ѽ���
	char	bvols[9];		// �ż�ȣ���Ѽ���
	char	ahvol[9];		// �ð��ܸŵ�����
	char	bhvol[9];		// �ð��ܸż�����
	char	acha[9];		// �ŵ�ȣ���Ѽ��� �������
	char	bcha[9];		// �ż�ȣ���Ѽ��� �������
	char	ahvc[9];		// �ð��ܸŵ����� �������
	char	bhvc[9];		// �Ⱓ�ܸż����� �������
	char	dscha[9];		// �ܷ���
	char	gsm5[9];		// 4�� ������
	char	gsm10[9];		// 9�� ������
	char	jivl[12];		// ���� �ŷ���
	char	jggy[6];		// ���ű���
	char	res2[9];		// 2������
	char	res1[9];		// 1������
	char	pivt[9];		// �Ǻ���
	char	spp1[9];		// 1������
	char	spp2[9];		// 2������ 
	char	resd[9];		// D����
	char	sppd[9];		// D����
};
#define sz_hoga	sizeof(struct _hoga)

struct	_hogax {
	char	codx[12];
	char	curr[8];
	char	diff[8];		// ���
	char	udyl[8];		// �����
	char	pcls[8];
	char	gvol[12];
	char	gamt[12];
        char    siga[8];
        char    koga[8];
        char    jega[8];
        char    amga[8];
	char	ysga[8];		
	char	yvol[9];	
	char	htim[8];
	char    gjga[8];                // ���ذ�
        char    pgga[8];                // ������հ�
        char    sjrt[6];                // ������       999V99
        char    shga[8];                // ���Ѱ�
        char    hhga[8];                // ���Ѱ�
	struct	_price	price[5];	// 5ȣ��
	char	avols[9];
	char	bvols[9];
	char	ahvol[9];
	char	bhvol[9];
	char	acha[9];
	char	bcha[9];
	char	ahvc[9];
	char	bhvc[9];
	char	dscha[9];		// �ܷ���
	char	mgjv[9];		/* �̰��� ����			*/
	char	mgdf[9];		/* �̰��� ����			*/
	char	mgsi[9];		/* �̰��� �÷�			*/
	char	mgko[9];		/* �̰��� ��			*/
	char	mgje[9];		/* �̰��� ����			*/
	char	ma05[9];		/* 5MA				*/
	char	sum4[9];		/* sum of 4 day			*/
	char	ma10[9];		/* 10MA				*/
	char	sum9[9];		/* sum of 9 day			*/
	char	base[9];		/* ���̽ý�  for ����		*/
	char	rate[6];		/* ������    for ����		*/
	char	irga[9];		/* �̷а�			*/
	char	njbd[9];		/* ���纯����		*/
	char	delt[9];		/* delta			*/
	char	gamm[9];		/* gamma			*/
	char	msha[9];		/* �ǽð� ���Ѱ� */
	char	mhha[9];		/* �ǽð� ���Ѱ� */
};
#define sz_hogax	sizeof(struct _hogax)

struct _Contract			// ü�� ���� 20080425
{
	char	time[8];
	char	price[8];
	char	contract[9];
};

struct _Candle				// �̴Ϻ� ���� 20080425
{
	char	date[8];
	char	siga[8];
	char	koga[8];
	char	jega[8];
	char	jgga[8];
	char	gvol[12];
};

struct _EW_data
{
	char	nContract[2];			// ü���
	char	nCandle[2];			// ĵ���
	struct _Contract	contract[15];	// ü�� ��Ʈ��
	struct _Candle		candle[30];	// �̴Ϻ���Ʈ
};
#define	sz_Extra	sizeof(struct _EW_data)


struct  grid_i {
        char    indx[1];                // index
        char    gubn[1];                // ���Ǳ���
                                        // 1: ���ذ�
                                        // 2: �Ⱓ
                                        // 3: Pivot
                                        // 4: �̵���ռ�
        char    dat1[1];
        char    dat2[8];
};
#define	sz_gridI	sizeof(struct grid_i)

struct  grid_o {
        char    indx[1];                // index
        char    gubn[1];                // ���Ǳ���
        char    vals[8];
};
#define sz_gridO	sizeof(struct grid_o)

#endif
