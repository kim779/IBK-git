#ifndef __C2440STRUCT_H
#define __C2440STRUCT_H

#pragma pack(push)
#pragma pack(1)

//{{JS.Kim_20100906 ����Ÿ�߰��θ���
#ifndef ChartKeySize
#define		ChartKeySize		28
#endif
//}}

//TR o44003$

typedef struct _HCQ03010_IN
{
	char		keyvalue[84];		/* key ����+�ð�	: key ����+�ð� */
	char		jmcode[10];			/* �����ڵ�			: �����ڵ� */
	char		date[8];			/* ����				: ���� */
	char		chetime[10];		/* �ð�				: �ð� */
	char		gubun[1];			/* ����				: 0:ó��1:���� */
	char		dgubun[1];			/* �����ͱ���		: 1:Tick,2:��,3:��,4:��,5:�� */
	char		nsel[4]/*[2]*/;		/* N�б���			: 0:30sec,1:1M,2:2M,... */
	char		dcnt[5];			/* �����ͰǼ�		: �����ͰǼ� */
//	char		qtycd[1];			/* 0: �������, 1:�������(�ǰŷ�������) */
    char		nflag[1];           /* ��ȸ���(1)                 */	////JS.Kim_20100906 ����Ÿ�߰��θ���
	char		nkey[ChartKeySize];	/* �˻�Ű(ChartKeySize)        */	////JS.Kim_20100906 ����Ÿ�߰��θ���
} HCQ03010_IN;

#define HCQ03010_IN_SZ sizeof(HCQ03010_IN)

typedef struct _o44003_OUT_HEADER
{
	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
	char		count[4];
} o44003_OUT_HEADER;
#define o44003_OUT_SZ sizeof(o44003_OUT_HEADER)

typedef struct _HCQ03010_OUT
{
	char		date[8];			/* ���� 					: ���� */
	char		chetime[10];		/* �ð� 					: �ð� */
	char		kdate[8];        //�߰�
	char		kchetime[10];    //�߰�
	char		open_d[10];	//15		/* ������ȯ�� �ð�			: ������ȯ�� �ð� */
	char		high_d[10];	//15		/* ������ȯ�� ��			: ������ȯ�� �� */
	char		low_d[10];	//15		/* ������ȯ�� ����			: ������ȯ�� ���� */
	char		price_d[10];//15		/* ������ȯ�� ����			: ������ȯ�� ���� */
	char		sign[1];			/* ���ϴ�񱸺�				: ���ϴ�񱸺� */
	char		change_d[10]; //10		/* ������ȯ�� ���ϴ��		: ������ȯ�� ���ϴ�� */
	char		drate[10];			/* ���ϴ������			: ���ϴ������ */
	char		presign[1];			/* ������񱸺�				: ������񱸺�("+"���"-"�϶�" "����) */
	char		cvolume[10];		/* ü�ᷮ					: ü�ᷮ */
	char		cgubun[1];			/* ü�ᱸ��("+"�ż�"-"�ŵ�) : ü�ᱸ��("+"�ż�"-"�ŵ�) */
	char		volume[10];			/* �����ŷ���				: �����ŷ��� */

} HCQ03010_OUT;

#define HCQ03010_OUT_SZ sizeof(HCQ03010_OUT)

//KTB�� OUT_KEY Packet ���� ��û �ؾ���.
typedef struct _HCQo44000_IN
{
	char	jmcode[10];
}HCQo44000_IN;  //_HCQ03010_OUT_KEY �� ��ǲ TR��� ����ü�� �ٸ�. 
#define HCQo44000_IN_SZ sizeof(HCQo44000_IN)

typedef struct _HCQ03010_OUT_KEY
{
	char		jmcode[10];		/* �����ڵ� */
	char		jmname[50];		/* ����� */
	char		price[10];		/* ���簡 */	
	char		sign[1];		/* ���ϴ�񱸺� */
	char		change[10];		/* ���ϴ�� */
	char		drate[10];		/* ���ϴ������ */
	char		presign[1];		/* ������񱸺� */
	char		open[10];		/* �ð� */
	char		high[10];		/* �� */
	char		low[10];		/* ���� */
	char		lthigh[10];		/* LT�� */
	char		lthighdate[8];		/* LT���� */
	char		ltlow[10];		/* LT���� */
	char		ltlowdate[8];		/* LT������ */
	char		bday[8];		/* ������ */						//	GMT �ð� ����
	char		ssvolume[10];		/* ��°ŷ��� */
	char		hrvolume[10];		/* �϶��ŷ��� */
	char		volume[10];		/* �����ŷ��� */
	char		lastday[8];		/* ü��ð� */
	char		jsprice[10];		/* ���갡 */					//	���ذ��� �����
	char		jsday[8];		/* ������ */
	char		jandaycnt[5];		/* �����ϼ� */
	char		mdhoga[10];		/* �ŵ�ȣ�� */
	char		mshoga[10];		/* �ż�ȣ�� */
	char		baseprice[10];  //���ذ�
	char		volrate[10];	//���ϰŷ��������

//	char		timeindex[2];		/* TIMEZONE �ε��� */
	char		date[8];			/* ���� */
	char		time[10];			/* �ð� */
	char		exchkind[1];		/* �ŷ��ұ��� 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
	char		estime[4];		/* ������(ETH) ����(HHMM) */		//	GMT �ð� ����
	char		eetime[4];		/* ������(ETH) ����(HHMM) */		//	GMT �ð� ����
	char		chetime_g[10];  //�߰�
}HCQ03010_OUT_KEY;

#define HCQ03010_OUT_KEY_SZ sizeof(HCQ03010_OUT_KEY)



//20091117 SJ_KIM
/******************************************************************************/
/*  HCQ03010  ���� ( ƽ, ��, ��, ��, �� )  TR I/O                             */
/******************************************************************************/
// typedef struct _HCQ03010_IN
// {
// 	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
// 	char		jmcode[10];			/* �����ڵ�			: �����ڵ� */
// 	char		date[8];			/* ����				: ���� */
// 	char		chetime[10];		/* �ð�				: �ð� */
// 	char		gubun[1];			/* ����				: 0:ó��1:���� */
// 	char		dgubun[1];			/* �����ͱ���		: 1:Tick,2:��,3:��,4:��,5:�� */
// 	char		nsel[3];			/* N�б���			: 0:30sec,1:1M,2:2M,... */
// 	char		dcnt[5];			/* �����ͰǼ�		: �����ͰǼ� */
// // --> [Edit]  ������ 2008/10/20	( HCQ3005���� )
// 	char		qtycd[1];			/* 0: �������, 1:�������(�ǰŷ�������) */
// // <-- [Edit]  ������ 2008/10/20
// 
// } HCQ03010_IN;
// 
// #define HCQ03010_IN_SZ sizeof(HCQ03010_IN)
// 
// typedef struct _HCQ03010_OUT
// {
// 	char		date[8];			/* ���� 					: ���� */
// 	char		chetime[10];		/* �ð� 					: �ð� */
// 	char		open_d[15];			/* ������ȯ�� �ð�			: ������ȯ�� �ð� */
// 	char		high_d[15];			/* ������ȯ�� ��			: ������ȯ�� �� */
// 	char		low_d[15];			/* ������ȯ�� ����			: ������ȯ�� ���� */
// 	char		price_d[15];		/* ������ȯ�� ����			: ������ȯ�� ���� */
// 	char		sign[1];			/* ���ϴ�񱸺�				: ���ϴ�񱸺� */
// 	char		change_d[15];		/* ������ȯ�� ���ϴ��		: ������ȯ�� ���ϴ�� */
// 	char		drate[10];			/* ���ϴ������			: ���ϴ������ */
// 	char		presign[1];			/* ������񱸺�				: ������񱸺�("+"���"-"�϶�" "����) */
// 	char		cvolume[10];		/* ü�ᷮ					: ü�ᷮ */
// 	char		cgubun[1];			/* ü�ᱸ��("+"�ż�"-"�ŵ�) : ü�ᱸ��("+"�ż�"-"�ŵ�) */
// 	char		volume[10];			/* �����ŷ���				: �����ŷ��� */
// 
// } HCQ03010_OUT;
// 
// #define HCQ03010_OUT_SZ sizeof(HCQ03010_OUT)
// 
// //KTB�� OUT_KEY Packet ���� ��û �ؾ���.
// typedef struct _HCQo44000_IN
// {
// 	char	jmcode[10];
// }HCQo44000_IN;  //_HCQ03010_OUT_KEY �� ��ǲ TR��� ����ü�� �ٸ�. 
// 
// typedef struct _HCQ03010_OUT_KEY
// {
// 	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
// 
// // --> [Edit]  ������ 2008/10/20	( HCQ3005���� )
// 	char		timeindex[2];		/* TIMEZONE �ε��� */
// 	char		date[8];			/* ���� */
// 	char		time[6];			/* �ð� */
// 	char		summertime[1];		/* SUMMER TIME���� 1:����Ÿ��, 0:��� */
// 	char		exchkind[1];		/* �ŷ��ұ��� 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
// 
// 	char		jmcode[10];		/* �����ڵ� */
// 	char		jmname[50];		/* ����� */
// 	char		estime[6];		/* ������(ETH) ����(HHMM) */		//	GMT �ð� ����
// 	char		eetime[6];		/* ������(ETH) ����(HHMM) */		//	GMT �ð� ����
// 	char		price[10];		/* ���簡 */
// 	char		sign[1];		/* ���ϴ�񱸺� */
// 	char		change[10];		/* ���ϴ�� */
// 	char		drate[10];		/* ���ϴ������ */
// 	char		presign[1];		/* ������񱸺� */
// 	char		open[10];		/* �ð� */
// 	char		high[10];		/* �� */
// 	char		low[10];		/* ���� */
// 	char		lthigh[10];		/* LT�� */
// 	char		lthighdate[8];		/* LT���� */
// 	char		ltlow[10];		/* LT���� */
// 	char		ltlowdate[8];		/* LT������ */
// 	char		bday[8];		/* ������ */						//	GMT �ð� ����
// 	char		ssvolume[10];		/* ��°ŷ��� */
// 	char		hrvolume[10];		/* �϶��ŷ��� */
// 	char		volume[10];		/* �����ŷ��� */
// 	char		lastday[8];		/* ü��ð� */
// 	char		jsprice[10];		/* ���갡 */					//	���ذ��� �����
// 	char		jsday[8];		/* ������ */
// 	char		jandaycnt[5];		/* �����ϼ� */
// 	char		mdhoga[10];		/* �ŵ�ȣ�� */
// 	char		mshoga[10];		/* �ż�ȣ�� */
// // <-- [Edit]  ������ 2008/10/20
// 
// 	char		display_cnt[ 5];	/* ��ȸ ����Ʈ COUNT    */
// 
// }HCQ03010_OUT_KEY;
// 
// #define HCQ03010_OUT_KEY_SZ sizeof(HCQ03010_OUT_KEY)




/******************************************************************************/
/*  HCQ03011	ȣ��íƮ   TR I/O			                          */
/******************************************************************************/
//TR o56501$
typedef struct _HCQ03011_IN
{
	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
	char		jmcode[10];			/* �����ڵ�			: �����ڵ� */
	char		date[8];			/* ����				: ���� */
	char		chetime[10];		/* �ð�				: �ð� */
	char		dcnt[5];			/* �����ͰǼ�		: �����ͰǼ� */

} HCQ03011_IN;
#define HCQ03011_IN_SZ sizeof(HCQ03011_IN)

typedef struct _o56501_OUT_HEADER
{
	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
	char		count[4];
} o56501_OUT_HEADER;
#define o56501_OUT_SZ sizeof(o56501_OUT_HEADER)

typedef struct _HCQ03011_OUT
{
	char		date[8];			/* ����					: ���� */
	char		chhotime[10];		/* �ð�					: �ð� */
	char		kdate[8];           //�߰�
	char		kchhotime[10];		//��
	char		mdhoga_d[15];		/* ������ȯ�� �ŵ�ȣ��	: ������ȯ�� �ŵ�ȣ�� */
	char		mdhorem[10];		/* �ŵ��ܷ�				: �ŵ��ܷ� */
	char		mshoga_d[15];		/* ������ȯ�� �ż�ȣ��	: ������ȯ�� �ż�ȣ�� */
	char		mshorem[10];		/* �ż��ܷ�				: �ż��ܷ� */

} HCQ03011_OUT;
#define HCQ03011_OUT_SZ sizeof(HCQ03011_OUT)

typedef struct _HCQ03011_OUT_KEY
{
	char		jmcode[10];		/* �����ڵ� */
	char		jmname[50];		/* ����� */
	char		price[10];		/* ���簡 */	
	char		sign[1];		/* ���ϴ�񱸺� */
	char		change[10];		/* ���ϴ�� */
	char		drate[10];		/* ���ϰ�������� */
	char		presign[1];		/* ������񱸺� */
	char		open[10];		/* �ð� */
	char		high[10];		/* �� */
	char		low[10];		/* ���� */
	char		lthigh[10];		/* LT�� */
	char		lthighdate[8];		/* LT���� */
	char		ltlow[10];		/* LT���� */
	char		ltlowdate[8];		/* LT������ */
	char		bday[8];		/* ������ */						//	GMT �ð� ����
	char		ssvolume[10];		/* ��°ŷ��� */
	char		hrvolume[10];		/* �϶��ŷ��� */
	char		volume[10];		/* �����ŷ��� */
	char		lastday[8];		/* ü��ð� */
	char		jsprice[10];		/* ���갡 */					//	���ذ��� �����
	char		jsday[8];		/* ������ */
	char		jandaycnt[5];		/* �����ϼ� */
	char		mdhoga[10];		/* �ŵ�ȣ�� */
	char		mshoga[10];		/* �ż�ȣ�� */
	char		baseprice[10];  //���ذ�
	char		volrate[10];	//���ϰŷ��������

//	char		timeindex[2];		/* TIMEZONE �ε��� */
	char		date[8];			/* ���� */
	char		time[10];			/* �ð� */
	char		exchkind[1];		/* �ŷ��ұ��� 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
	char		estime[4];		/* ������(ETH) ����(HHMM) */		//	GMT �ð� ����
	char		eetime[4];		/* ������(ETH) ����(HHMM) */		//	GMT �ð� ����
	char		chetime_g[10];  //�߰�
}HCQ03011_OUT_KEY;
#define HCQ03011_OUT_KEY_SZ sizeof(HCQ03011_OUT_KEY)
// typedef struct _HCQ03011_IN
// {
// 	char		keyvalue[18];		/* key ����+�ð�	: key ����+�ð� */
// 	char		jmcode[10];			/* �����ڵ�			: �����ڵ� */
// 	char		date[8];			/* ����				: ���� */
// 	char		chetime[10];		/* �ð�				: �ð� */
// 	char		dcnt[5];			/* �����ͰǼ�		: �����ͰǼ� */
// 
// } HCQ03011_IN;
// 
// #define HCQ03011_IN_SZ sizeof(HCQ03011_IN)
// 
// typedef struct _HCQ03011_OUT
// {
// 	char		date[8];			/* ����					: ���� */
// 	char		chhotime[10];		/* �ð�					: �ð� */
// //	char		mdhoga[10];			/* �ŵ�ȣ��				: �ŵ�ȣ�� */
// 	char		mdhoga_d[15];		/* ������ȯ�� �ŵ�ȣ��	: ������ȯ�� �ŵ�ȣ�� */
// 	char		mdhorem[10];		/* �ŵ��ܷ�				: �ŵ��ܷ� */
// //	char		mshoga[10];			/* �ż�ȣ��				: �ż�ȣ�� */
// 	char		mshoga_d[15];		/* ������ȯ�� �ż�ȣ��	: ������ȯ�� �ż�ȣ�� */
// 	char		mshorem[10];		/* �ż��ܷ�				: �ż��ܷ� */
// 
// } HCQ03011_OUT;
// 
// #define HCQ03011_OUT_SZ sizeof(HCQ03011_OUT)
// 
// 
// typedef struct _HCQ03011_OUT_KEY
// {
// 	char		keyvalue[18];			/* key ����+�ð�	: key ����+�ð� */
// 
// // --> [Edit]  ������ 2008/10/28
// 	char		timeindex[2];		/* TIMEZONE �ε��� */
// 	char		date[8];		/* ���� */
// 	char		time[6];		/* �ð� */
// 	char		summertime[1];		/* SUMMER TIME���� 1:����Ÿ��, 0:��� */
// 	char		exchkind[1];		/* �ŷ��ұ��� 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
// 
// 	char		jmcode[10];		/* �����ڵ� */
// 	char		jmname[50];		/* ����� */
// 	char		estime[6];		/* ������(ETH) ����(HHMM) */
// 	char		eetime[6];		/* ������(ETH) ����(HHMM) */
// 	char		price[10];		/* ���簡 */
// 	char		sign[1];		/* ���ϴ�񱸺� */
// 	char		change[10];		/* ���ϴ�� */
// 	char		drate[10];		/* ���ϴ������ */
// 	char		presign[1];		/* ������񱸺� */
// 	char		open[10];		/* �ð� */
// 	char		high[10];		/* �� */
// 	char		low[10];		/* ���� */
// 	char		lthigh[10];		/* LT�� */
// 	char		lthighdate[8];		/* LT���� */
// 	char		ltlow[10];		/* LT���� */
// 	char		ltlowdate[8];		/* LT������ */
// 	char		bday[8];		/* ������ */
// 	char		ssvolume[10];		/* ��°ŷ��� */
// 	char		hrvolume[10];		/* �϶��ŷ��� */
// 	char		volume[10];		/* �����ŷ��� */
// 	char		lastday[8];		/* ü��ð� */
// 	char		jsprice[10];		/* ���갡 */
// 	char		jsday[8];		/* ������ */
// 	char		jandaycnt[5];		/* �����ϼ� */
// 	char		mdhoga[10];		/* �ŵ�ȣ�� */
// 	char		mshoga[10];		/* �ż�ȣ�� */
// // <-- [Edit]  ������ 2008/10/28
// 
//     char		display_cnt[ 5];		/* ��ȸ ����Ʈ COUNT    */
// }HCQ03011_OUT_KEY;
// 
// #define HCQ03011_OUT_KEY_SZ sizeof(HCQ03011_OUT_KEY)


/******************************************************************************/
/*  HCQ01120  ���簡�ü�  TR I/O										      */
/******************************************************************************/
typedef struct _HCQ01120_IN
{
	char		jmcode[10];			/* �����ڵ� : | */

} HCQ01120_IN;

#define HCQ01120_IN_SZ sizeof(HCQ01120_IN)

typedef struct _HCQ01120_OUT
{
	char		jmcode[10];			/* �����ڵ� : �����ڵ� */
	char		jmname[50];			/* ����� : | */
	char		price[10];			/* ���簡 : ���簡 */
	char		sign[1];			/* ���ϴ�񱸺� : ���ϴ�񱸺� */
	char		change[10];			/* ���ϴ�� : ���ϴ�� */
	char		drate[10];			/* ���ϴ������ : ���ϴ������ */
	char		presign[1];			/* ������񱸺� : ������񱸺� */
	char		open[10];			/* �ð� : �ð� */
	char		high[10];			/* �� : �� */
	char		low[10];			/* ���� : ���� */
	char		lthigh[10];			/* LT�� : LT�� */
	char		lthighdate[8];		/* LT���� : LT���� */
	char		ltlow[10];			/* LT���� : LT���� */
	char		ltlowdate[8];		/* LT������ : LT������ */
	char		bday[8];			/* ������ : ������ */
	char		ssvolume[10];		/* ��°ŷ��� : ��°ŷ��� */
	char		hrvolume[10];		/* �϶��ŷ��� : �϶��ŷ��� */
	char		volume[10];			/* �����ŷ��� : �����ŷ��� */
	char		chetime[8];			/* ü��ð� : ü��ð� */
	char		jsprice[10];		/* ���갡 : ���갡 */
	char		jsday[8];			/* ������ : ������ */
	char		jandaycnt[5];		/* �����ϼ� : �����ϼ� */
	char		mdhoga[10];			/* �ŵ�ȣ�� : �ŵ�ȣ�� */
	char		mshoga[10];			/* �ż�ȣ�� : �ż�ȣ�� */
	
} HCQ01120_OUT;	
	
#define HCQ01120_OUT_SZ sizeof(HCQ01120_OUT)


/******************************************************************************/
/*  HCQ01112  ȣ������  TR I/O										      */
/******************************************************************************/
typedef struct _HCQ01112_IN
{
	char		jmcode[10];			/* �����ڵ� : | */

} HCQ01112_IN;

#define HCQ01112_IN_SZ sizeof(HCQ01112_IN)

typedef struct _HCQ01112_OUT
{
	char		jmcode[10];			/* �����ڵ� : �����ڵ� */
	char		hotime[8];			/* ȣ���ð� : ȣ���ð� */
	char		mdhoga1[10];		/* �ŵ�ȣ��1 : �ŵ�ȣ��1 */
	char		mshoga1[10];		/* �ż�ȣ��1 : �ż�ȣ��1 */
	char		mdhorem1[10];		/* �ŵ�ȣ���ܷ�1 : �ŵ�ȣ���ܷ�1 */
	char		mshorem1[10];		/* �ż�ȣ���ܷ�1 : �ż�ȣ���ܷ�1 */
	char		mdhocnt1[10];		/* �ŵ�ȣ���Ǽ�1 : �ŵ�ȣ���Ǽ�1 */
	char		mshocnt1[10];		/* �ż�ȣ���Ǽ�1 : �ż�ȣ���Ǽ�1 */
	char		mdhoga2[10];		/* �ŵ�ȣ��2 : �ŵ�ȣ��2 */
	char		mshoga2[10];		/* �ż�ȣ��2 : �ż�ȣ��2 */
	char		mdhorem2[10];		/* �ŵ�ȣ���ܷ�2 : �ŵ�ȣ���ܷ�2 */
	char		mshorem2[10];		/* �ż�ȣ���ܷ�2 : �ż�ȣ���ܷ�2 */
	char		mdhocnt2[10];		/* �ŵ�ȣ���Ǽ�2 : �ŵ�ȣ���Ǽ�2 */
	char		mshocnt2[10];		/* �ż�ȣ���Ǽ�2 : �ż�ȣ���Ǽ�2 */
	char		mdhoga3[10];		/* �ŵ�ȣ��3 : �ŵ�ȣ��3 */
	char		mshoga3[10];		/* �ż�ȣ��3 : �ż�ȣ��3 */
	char		mdhorem3[10];		/* �ŵ�ȣ���ܷ�3 : �ŵ�ȣ���ܷ�3 */
	char		mshorem3[10];		/* �ż�ȣ���ܷ�3 : �ż�ȣ���ܷ�3 */
	char		mdhocnt3[10];		/* �ŵ�ȣ���Ǽ�3 : �ŵ�ȣ���Ǽ�3 */
	char		mshocnt3[10];		/* �ż�ȣ���Ǽ�3 : �ż�ȣ���Ǽ�3 */
	char		mdhoga4[10];		/* �ŵ�ȣ��4 : �ŵ�ȣ��4 */
	char		mshoga4[10];		/* �ż�ȣ��4 : �ż�ȣ��4 */
	char		mdhorem4[10];		/* �ŵ�ȣ���ܷ�4 : �ŵ�ȣ���ܷ�4 */
	char		mshorem4[10];		/* �ż�ȣ���ܷ�4 : �ż�ȣ���ܷ�4 */
	char		mdhocnt4[10];		/* �ŵ�ȣ���Ǽ�4 : �ŵ�ȣ���Ǽ�4 */
	char		mshocnt4[10];		/* �ż�ȣ���Ǽ�4 : �ż�ȣ���Ǽ�4 */
	char		mdhoga5[10];		/* �ŵ�ȣ��5 : �ŵ�ȣ��5 */
	char		mshoga5[10];		/* �ż�ȣ��5 : �ż�ȣ��5 */
	char		mdhorem5[10];		/* �ŵ�ȣ���ܷ�5 : �ŵ�ȣ���ܷ�5 */
	char		mshorem5[10];		/* �ż�ȣ���ܷ�5 : �ż�ȣ���ܷ�5 */
	char		mdhocnt5[10];		/* �ŵ�ȣ���Ǽ�5 : �ŵ�ȣ���Ǽ�5 */
	char		mshocnt5[10];		/* �ż�ȣ���Ǽ�5 : �ż�ȣ���Ǽ�5 */
	char		totmdhorem[10];		/* �Ѹŵ�ȣ���ܷ� : �Ѹŵ�ȣ���ܷ� */
	char		totmshorem[10];		/* �Ѹż�ȣ���ܷ� : �Ѹż�ȣ���ܷ� */
	char		totmdhocnt[10];		/* �Ѹŵ�ȣ���Ǽ� : �Ѹŵ�ȣ���Ǽ� */
	char		totmshocnt[10];		/* �Ѹż�ȣ���Ǽ� : �Ѹż�ȣ���Ǽ� */
	char		open[10];			/* �ð� : �ð� */
	char		high[10];			/* �� : �� */
	char		low[10];			/* ���� : ���� */
	char		price[10];			/* ���簡 : ���簡 */
	char		sign[1];			/* ���ϴ�񱸺� : ���ϴ�񱸺� */
	char		change[10];			/* ���ϴ�� : ���ϴ�� */
	char		presign[1];			/* ������񱸺� : ������񱸺� */
	char		jsprice[10];		/* ���갡 : ���갡 */
	char		volume[10];			/* �����ŷ��� : �����ŷ��� */
	char		drate[10];			/* ���ϴ������ : ���ϴ������ */

	char		ho_drate1[10];		/*  [�ŵ�ȣ�������1]�ŵ�ȣ�������1     */
	char		hb_drate1[10];		/*  [�ż�ȣ�������1]�ŵ�ȣ�������2     */
	char		ho_drate2[10];		/*  [�ŵ�ȣ�������2]�ŵ�ȣ�������3     */
	char		hb_drate2[10];		/*  [�ż�ȣ�������2]�ŵ�ȣ�������4     */
	char		ho_drate3[10];		/*  [�ŵ�ȣ�������3]�ŵ�ȣ�������5     */
	char		hb_drate3[10];		/*  [�ż�ȣ�������3]�ż�ȣ�������1     */
	char		ho_drate4[10];		/*  [�ŵ�ȣ�������4]�ż�ȣ�������2     */
	char		hb_drate4[10];		/*  [�ż�ȣ�������4]�ż�ȣ�������3     */
	char		ho_drate5[10];		/*  [�ŵ�ȣ�������5]�ż�ȣ�������4     */
	char		hb_drate5[10];		/*  [�ż�ȣ�������5]�ż�ȣ�������5     */
	char		tot_bidrem[10];		/*  [�Ѽ��ż��ܷ�]�Ѽ��ż��ܷ�     */
	char		totofferremcha[10];		/*  [�����ŵ�����Ѽ���]�����ŵ�����Ѽ���     */
	char		totbidremcha[10];		/*  [�����ż�����Ѽ���]�����ż�����Ѽ���     */
	char		totoffercntcha[10];		/*  [�����ŵ�����ѰǼ�]�����ŵ�����ѰǼ�     */
	char		totbidcntcha[10];		/*  [�����ż�����ѰǼ�]�����ż�����ѰǼ�     */
	
} HCQ01112_OUT;	
	
#define HCQ01112_OUT_SZ sizeof(HCQ01112_OUT)


// --> [Edit]  ������ 2009/08/23
//======================================================================
// �������� ��Ʈ (HCQ03050)
//======================================================================

typedef struct _HCQ03050_IN
{
	char		keyvalue[18];		/* ���� */
	char		jmcode1[10];		/* �����ڵ� 1 */
	char		jmcode2[10];		/* �����ڵ� 2 */
	char		date[8];		/* ���� */
	char		chetime[10];		/* �ð� */
	char		gubun[1];		/* 0:ó��,1:���� */
	char		dgubun[1];		/* 1:Tick,2:��,3:��,4:��,5:�� */
	char		nsel[3];		/* 0:30sec,1:1M,2:2M,... */
	char		dcnt[5];		/* �����ͰǼ� */
}HCQ03050_IN;
#define HCQ03050_IN_SZ sizeof(HCQ03050_IN)


typedef struct _HCQ03050_OUT1_KEY
{
	char		keyvalue[18];		/* key ����+�ð� */
	char		display_cnt[ 5];	/* ��ȸ ����Ʈ COUNT    */

}HCQ03050_OUT1_KEY;
#define HCQ03050_OUT1_KEY_SZ sizeof(HCQ03050_OUT1_KEY)


typedef struct _HCQ03050_OUT1
{
	char		date1[8];		/* ���� */
	char		chetime1[10];		/* �ð� */
	char		open_d1[15];		/* ������ȯ�� �ð� */
	char		high_d1[15];		/* ������ȯ�� �� */
	char		low_d1[15];		/* ������ȯ�� ���� */
	char		price_d1[15];		/* ������ȯ�� ���� */
	char		sign1[1];		/* ���ϴ�񱸺� */
	char		change_d1[15];		/* ������ȯ�� ���ϴ�� */
	char		drate1[10];		/* ���ϴ������ */
	char		presign1[1];		/* ������񱸺�("+"���"-"�϶�" "����) */
	char		cvolume1[10];		/* ü�ᷮ */
	char		cgubun1[1];		/* ü�ᱸ��("+"�ż�"-"�ŵ�) */
	char		volume1[10];		/* �����ŷ��� */
}HCQ03050_OUT1;
#define HCQ03050_OUT1_SZ sizeof(HCQ03050_OUT1)


typedef struct _HCQ03050_OUT2_KEY
{
	char		display_cnt[ 5];	/* ��ȸ ����Ʈ COUNT    */

}HCQ03050_OUT2_KEY;
#define HCQ03050_OUT2_KEY_SZ sizeof(HCQ03050_OUT2_KEY)


typedef struct _HCQ03050_OUT2
{
	char		date2[8];		/* ���� */
	char		chetime2[10];		/* �ð� */
	char		open_d2[15];		/* ������ȯ�� �ð� */
	char		high_d2[15];		/* ������ȯ�� �� */
	char		low_d2[15];		/* ������ȯ�� ���� */
	char		price_d2[15];		/* ������ȯ�� ���� */
	char		sign2[1];		/* ���ϴ�񱸺� */
	char		change_d2[15];		/* ������ȯ�� ���ϴ�� */
	char		drate2[10];		/* ���ϴ������ */
	char		presign2[1];		/* ������񱸺�("+"���"-"�϶�" "����) */
	char		cvolume2[10];		/* ü�ᷮ */
	char		cgubun2[1];		/* ü�ᱸ��("+"�ż�"-"�ŵ�) */
	char		volume2[10];		/* �����ŷ��� */
}HCQ03050_OUT2;
#define HCQ03050_OUT2_SZ sizeof(HCQ03050_OUT2)

typedef struct _ST_CHART_DATA
{
	CString		szDate;			/* ���� */
	CString		szTime;			/* �ð� */
	CString		szOpen1;		/* ������ȯ�� �ð� */
	CString		szHigh1;		/* ������ȯ�� �� */
	CString		szLow1;			/* ������ȯ�� ���� */
	CString		szClose1;		/* ������ȯ�� ���� */

	CString		szOpen2;		/* ������ȯ�� �ð� */
	CString		szHigh2;		/* ������ȯ�� �� */
	CString		szLow2;			/* ������ȯ�� ���� */
	CString		szClose2;		/* ������ȯ�� ���� */

}ST_CHART_DATA;
//======================================================================
// <-- [Edit]  ������ 2009/08/23

#pragma pack(pop)

#endif