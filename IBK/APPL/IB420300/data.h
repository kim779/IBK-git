#pragma once


typedef struct _jcode
{
	char	indx;		/* ��ȣ			*/
	char	code[12];	/* �����ڵ�		*/
	char	cod2[20];	/* �����		*/
	char	giho;		/* ��ȣ			*/
	char	udyl[7];	/* ���ϴ��		*/
	char	bdyl[7];	/* ��������		*/
	char	sdyl[7];	/* �ð�����		*/
}_jcode2;

typedef struct _outbound
{
	char	djis[7];	/* �϶�����		*/
	char	dnyl[7];	/* �϶�����		*/
	char	dnum[7];	/* �϶������		*/
	char	ujis[7];	/* �������		*/
	char	upyl[7];	/* ��º���		*/
	char	unum[7];	/* ��������		*/
	struct _jcode	ucod[10];/* ��� ����		*/
	struct _jcode	dcod[10];/* �϶� ����		*/
	char	rank[4];	/* rank			*/
	char	time[6];	/* �ð�			*/
}_outbound2;

