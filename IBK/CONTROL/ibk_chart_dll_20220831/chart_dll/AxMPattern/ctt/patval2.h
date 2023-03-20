/*  Candle Pattern for Prudential securities	*/

/* ��¹�����	*/
#define V2_InvertedHammer		101
#define V2_Hammer			102
#define V2_BullishBeltHold		103
#define V2_BullishEngulfing		104
#define V2_Harami			105
#define V2_HaramiCross			106
#define V2_BullishCounterAttckLine	107
#define V2_PiercingLine			108
#define V2_MorningStar			109
#define V2_MorningDojiStar		110
#define V2_AbandonedBaby		111
#define V2_ThreeInsideUp		112
#define V2_ThreeOutsideUp		113
#define V2_UpsideGap2CrowsDown		114
#define V2_BullishDojiStar		115
#define V2_TweezersBottom		116

/* ���������	*/
#define V2_ThreeWhiteSoldier		151
#define V2_AdvancedBlock		152
#define V2_StalledPattern		153
#define V2_UpsideGapTasuki		154
#define V2_UpsideGap3Method		155

/* �϶�������	*/
#define V2_HangingMan			201
#define V2_BearishBeltHold		202
#define V2_BearishEngulfing		203
#define V2_HaramiBlack			204
#define V2_HaramiCrossBlack		205
#define V2_BearishCounterAttckLine	206
#define V2_DarkCrowsCover		207
#define V2_EveningStar			208
#define V2_EveningDojiStar		209
#define V2_AbandonedBabyBlack		210
#define V2_ThreeInsideDown		211
#define V2_ThreeOutsideDown		212
#define V2_UpsideGap2Crows		213
#define V2_BearishDojiStar		214
#define V2_TweezersTop			215

/* �϶�������	*/
#define V2_ThreeBlackCrows		251
#define V2_IdenticalThreeCrows		252
#define V2_DownsideGapTasuki		253
#define V2_DownsideGap3Method		254
#define V2_ShootingStar			255

/* ��¹�����	*/
char	*to_up[20] = {
	"", "����ġ�� ", "�����ġ�� ", "�Ҹ�����ƮȦ�� ", "�������� ",
	"��������� ", "�϶����������� ", "��������� ", "������ ",
	"������ ", "���������� ", "�����庣�̺� ", "���� �λ��̵�� ",
	"���� �ƿ����̵�� ", "�϶������ ", "�����������Ÿ ",
	"�ٴ������� ",
	"END",
};
/* ���������	*/
char	*con_up[20] = {
	"", "����ȭ��Ʈ���� ", "���꽺�Ҷ� ", "��������� ", "�����̵��½�Ű ",
	"�����̵尸���� ",
	"END",
};
/* �϶�������	*/
char	*to_down[20] = {
	"", "������ġ�� ", "�������ƮȦ�� ", "�϶������ ", "�϶������� ",
	"��½��������� ", "�϶������� ", "�Ա����� ", "������ ", "���������� ",
	"�����庣�̺� ", "�����λ��̵�ٿ� ", "�����ƿ����̵�ٿ� ", "��±���� ",
	"�����������Ÿ ", "õ�������� ",
	"END",
};
/* �϶�������	*/
char	*con_down[20] = {
	"", "������ũ�ο��� ", "���̵�Ƽ�þ���ũ���� ", "�϶����İ��� ",
	"�ٿ���̵尸���� ", "��������ġ�� ",
	"END",
};

#define	V_AbandonedBabyBottom	1	/* ��±����		*/
#define	V_AbandonedBabyTop	2	/* �϶������		*/
#define	V_AdvanceBlock		3	/* ��º���		*/
#define	V_CounterAttackBear	4	/* �϶��ݰ���		*/
#define	V_CounterAttackBull	5	/* ��¹ݰ���		*/
#define	V_DarkCloudCover	6	/* �Ա�����		*/
#define	V_EngulfingLineBear	7	/* �϶������		*/
#define	V_EngulfingLineBull	8	/* ��������		*/
#define	V_EveningDojiStar	9	/* ���ڼ�����		*/
#define	V_EveningStar		10	/* ������		*/
#define	V_Hammer		11	/* ��ġ��		*/
#define	V_Hangingman		12	/* ������		*/
#define	V_HaramiBlack		13	/* ������		*/
#define	V_HaramiCrossBlack	14	/* �϶�����������	*/
#define	V_HaramiCrossWhite	15	/* ��½���������	*/
#define	V_HaramiWhite		16	/* ���������		*/
#define	V_InvertedHammer	17	/* ����ġ��		*/
#define	V_MorningDojiStar	18	/* ���ڻ�����		*/
#define	V_MorningStar		19	/* ������		*/
#define	V_PiercingLine		20	/* ������		*/
#define	V_SeperatingLineBear	21	/* �϶��ݸ���		*/
#define	V_SeperatingLineBull	22	/* ��°ݸ���		*/
#define	V_ShootingStar		23	/* ������		*/
#define	V_SideBySideWhiteGapDn	24	/* �϶���������		*/
#define	V_SideBySideWhiteGapUp	25	/* ��³�������		*/
#define	V_TasukiDownsideGap	26	/* �϶�Ÿ��Ű����	*/
#define	V_TasukiUpsideGap	27	/* ���Ÿ��Ű����	*/
#define	V_ThreeWhiteSoldiers	28	/* ���ﺴ		*/
#define	V_ThrustingLine		29	/* ��°�����		*/
#define	V_UpsideGap2Crows	30	/* �����		*/

char	*pattern_name[] = {
	"",
	"��±���� ", "�϶������ ", "��º��� ", "�϶��ݰ��� ",
	"��¹ݰ��� ", "�Ա����� ", "�϶������ ", "�������� ",
	"���ڼ����� ", "������ ", "��ġ�� ", "������ ",
	"������ ", "�϶����������� ", "��½��������� ", "��������� ",
	"����ġ�� ", "���ڻ����� ", "������ ", "������",
	"�϶��ݸ��� ", "��°ݸ��� ", "������ ", "�϶��������� ",
	"��³������� ", "�϶�Ÿ��Ű���� ", "���Ÿ��Ű���� ", "���ﺴ ",
	"��°����� ", "����� ",
};

#define	N_PATTERN	10

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

