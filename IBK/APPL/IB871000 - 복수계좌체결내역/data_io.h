#pragma once

#define TK_PIDOSETA		0x01
#define TK_PIBOCHEG		0x02

struct  pidoseta_accn {
    char    seqn[3];        /* �����Ϸù�ȣ                 */
    char    accn[11];       /* ���¹�ȣ                     */
    char    pass[10];       /* ���º�й�ȣ                 */
    char    acnm[30];       /* ���¸�                       */
    char    rate[5];        /* ��������                     */
    char    maxq[7];        /* �ִ��ֹ����� for �����ֹ�    */
    char    alis[30];       /* ���º�Ī                     */
    char    pres[1];        /* 1:��ǥ����                   */
};

struct  pidoseta_grop {
    char    xflg[1];        /* 'D' Delete    'I' Insert     */
							/* 'U' Update or Still          */
							/* OUTPUT ���: group fg        */
    char    gpid[3];        /* ���±׷���̵�               */
    char    seqn[3];        /* ���±׷��Ϸù�ȣ             */
    char    gnam[20];       /* �׷��                       */
    char    nrec[4];        /* ���� COUNT                   */
    struct  pidoseta_accn  accn[1];
};

struct  pidoseta_mid {
    char    xflg[1];        /* 1: ��ȸ      2: ����         */
    char    usid[12];       /* ����ھ��̵�                 */
    char    sflg[1];        /* 1: �ֽ�                      */
							/* 2: ����/�ɼ�                 */
							/* 3: ��ü                      */
    char    grec[4];        /* �׷찹��                     */
    char    data[1];
};

struct  pidoseta_mod {
    char    okgb[1];        /* 'Y':���� , 'N':����          */
    char    grec[4];        /* �׷찹��                     */
    char    data[1];
};

struct	piboscheg_mid	
{
	char    gubn[1];	//2015.05.28 KSJ ������ '>' �־���.
	char    accn[11];
	char    pswd[8];
	char    date[8];
	char    cod2[12];
	char    juno[5];	// �����ֺй�ȣ               //2015.05.28 KSJ ��ȣ 5 -> 10 
	char    dsgb[1];	// 0.All 1.�ŵ� 2.�ż�      
	char    otim[1];	// 0.All 1.���� 2.�ð���    
	char	sygb[1];	// 0.All 1.�ſ� 2.�㺸
	char	dllf[1];	// 0.MAP 1.DLL
	char    dlgb[1];	// 0.All 1.ü�� 2.��ü��    
	char    sort[1];	// 1.���� 2.����            
	char    juno2[10];	// �����ֺй�ȣ2               //2015.05.28 KSJ ��ȣ 5 -> 10 
};
#define sz_MICHEMID		sizeof(struct piboscheg_mid)

struct	piboscheg_grid	{
	char    juno[10];    // �ֹ�   ��ȣ      //2015.05.28 KSJ ��ȣ 5 -> 10 
	char    ojno[10];    // ���ֹ� ��ȣ      //2015.05.28 KSJ ��ȣ 5 -> 10 
	char    cod2[12];   // �����ڵ�         
	char    hnam[40];   // �����           
	char    odgb[20];   // �ֹ�����         
	char    jcgb[20];   // ������ұ��� 
	char    hogb[20];   // ȣ������         
	char    jprc[12];   // �ֹ�����         
	char    jqty[12];   // �ֹ�����         
	char    dprc[12];   // ü�ᰡ��         
	char    dqty[12];   // ü�����         
	char    tqty[12];   // ü�������       
	char    wqty[12];   // ��ü�����       
	char    stat[20];   // ��������         
	char    time[8];    // �ֹ��ð�         
};

struct	piboscheg_mod	
{
	char	accn[11];
	char	nrec[4];
	struct	piboscheg_grid	rec[250];
};

