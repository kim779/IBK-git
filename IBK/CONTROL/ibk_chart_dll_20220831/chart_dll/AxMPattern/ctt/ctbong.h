
#include <stdio.h>
#include <stdlib.h>

struct	yymd	{
	short	yy;
	char	mm;
	char	dd;
};
struct	hms	{
	char	hh;
	char	mm;
	char	ss;
};
typedef	struct	yymd	YMD;
typedef	struct	hms	HMS;

struct	bunbong	{
	YMD	date;		/* ����				*/
	HMS	time;		/* �ú�				*/
	int	siga;		/* �ð�				*/
	int	koga;		/* ��				*/
	int	jega;		/* ����				*/
	int	jgga;		/* ����				*/
	char	giho;		/* ��ȣ				*/
	int	diff;		/* ���				*/
	int	gvol;		/* �ŷ���			*/
	int	xvol;		/* ���� �ŷ���			*/
};

struct  bonginf {
	int	nrec;
	int	type;		/* 1:date,2:date+time,3:time */
	char	cod2[16];
	struct	bunbong bong[400];
};