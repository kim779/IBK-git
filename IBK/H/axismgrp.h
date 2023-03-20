#pragma once
#include "axisgenv.h"
#pragma pack(1)

// ��Ƽ ��Ʈ�� ���� ���� ����(PruWin 7530~7533)

#define	MAX_MMAPY		4					// Map�� �ִ� Y Size
#define	MAX_MMAPX		6					// Map�� �ִ� X Size
#define	MAX_MMAPCELL	MAX_MMAPY*MAX_MMAPX	// Map�� �ִ� Cell Size

#define	VERSION_MC		1					// Multi Chart ����
#define MCI				".mc0"				// Multi Chart ����

#define	MT_SINGLE	1	// �������� ��Ƽ��Ʈ
#define	MT_MULTI	2	// �������� ��Ƽ��Ʈ
#define	MT_MONITOR	3	// ���尨�� ��Ƽ��Ʈ
#define	MT_FOREIGN	4	// �ؿ����� ��Ƽ��Ʈ

struct _MultiChart
{
	WORD	wVersion{};						// file version - reserved
	char	acObjName[16]{};					// ObjName
	BYTE	btUnit{};							// �ʱ� ���� ����
	BYTE	btShowCFG{};						// ConfigBar Show
	BYTE	btOption{};						// Option
	BYTE	btType{};							// 1~4
	BYTE	btToday{};						// ����üũ 
	WORD	btMainKind{};						// Main Graph Kind
    char    acCntX{};							// ���� ���������(1~6)
    char    acCntY{};							// ���� ����������(1~4)
	int		acGigan[5]{};						// ��,��,��,��,ƽ(300,200,100,400,400)
	char	acUlist[MAX_UGRP]{};				// ���ã��
	char    acSelect[MAX_MMAPCELL]{};			// map CheckBox
    char    acMatrix[300]{};					// map matrix
    char    acCode[MAX_MMAPCELL][12]{};		// �ڵ� ����
	char	acDummy[64]{};					// reserve
};
#define	SZ_MULTICHART	sizeof(struct _MultiChart)

struct	_ActiveChartInfo
{
	char	acCode[12]{};						// �ڵ�
	int		iUnit{};							// ����,�ڵ�..
	int		iIndex{};							// ���ֿ�..
	int		iMGap{};							// �а�
	int		iTGap{};							// ƽ��
	int		iRCnt{};							// ��û ����
	int		iToday{};							// ���� üũ
};

#pragma pack()

