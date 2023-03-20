#pragma once

#define MAX_REQCOUNT	96
#define TR_STARTKEY		64
#define TR_WINEKEY		32

typedef struct _M_GRID_I
{
	char vrow[2];
	char nrow[1];
	char vflg[1];
	char gdir[1];
	char sdir[1];
	char scol[16];
	char ikey[1];
	char page[4];
	char save[80];
} M_GRID_I;

typedef struct _GRID_O
{
	char aflg[1];
	char sdir[1];
	char scol[16];
	char xpos[1];
	char page[4];
	char save[80];
} GRID_O;

//WINE����
typedef struct _ISACAQ601
{
	char zIn[5];
	char zHtsID[16];	//HTS ID
} ISACAQ601;

#define L_ISACAQ601 sizeof(ISACAQ601)

typedef struct _OSACAQ601
{
	char zOut[5];
	char zscvDataID[3];	//���Կ���[3]�ڸ� WINE-S��Ʈ, WINE-F��Ʈ, ����ö������ �ΰ�������Ʈ
} OSACAQ601;


