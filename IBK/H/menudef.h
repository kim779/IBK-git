#pragma once
/*
const int chartBASE = 3000;
const int chartTYPE = 3100;
const int chartAID  = 3200;

*/
#define MENU_CHART_BASE	3000
#define	MENU_CHART_TYPE	3100
#define	MENU_CHART_AID	3200

struct _menuString {
	UINT	uFlag{};
	int	nType{};
	int	nKind{};
	char	item[32]{};
};

struct	_menuString g_aMenuBase[] = {
	{MFT_STRING,	0,	0,	"�߼���"	},	// +0
	{MFT_STRING,	0,	0,	"���ڼ�"	},	// +1
	{MFT_STRING,	0,	0,	"����"	},	// +2
	{MFT_STRING,	0,	0,	"������"	},	// +3
	{MFT_STRING,	0,	0,	"��ü�����"	},	// +4
	{MFT_SEPARATOR,	0,	0,	""		},	// +5
	{MFT_STRING,	0,	0,	"��Ʈ����"	},	// +6
	{MFT_STRING,	0,	0,	"��ǥ�߰�"	},	// +7
	{MFT_STRING,	0,	0,	"��ġ��ȸâ ǥ��"},	// +8
	{MFT_STRING,	0,	0,	"�ڷ���ȸ"	},	// +9
	{MFT_STRING,	0,	0,	"�α�����"},		// +10
	{MFT_STRING,	0,	0,	"��Ī���"},		// +11
	{MFT_STRING,	0,	0,	"�ʱ�ȭ"	},	// +12
	{MFT_SEPARATOR,	0,	0,	""		},	// +13
	{MFT_STRING,	0,	0,	"ȭ�鼳��"	},	// +14
	{MFT_STRING,	0,	0,	"��ǥ����"	},	// +15
	{MFT_STRING,	0,	0,	"��ǥ����"	},	// +16
	{MFT_STRING,	0,	0,	"ȯ�漳��"	},	// +17
	//** macho add
	{MFT_SEPARATOR, 0,	0,	"" },			// 18
	{MFT_STRING,	0,	0,	"�ü� ���̱�"},		// 19
	{MFT_STRING,	0,	0,	"������ ���̱�" },	// 20
	{MFT_STRING,	0,	0,	"��ǥ�� ���̱�" },	// 21
	{MFT_STRING,	0,	0,	"������ ����"	},	// 22
	{MFT_STRING,	0,	0,	"��ǥ�� ����"	},	// 23
	{MFT_SEPARATOR,	0,	0,	""		},	// 24
	{MFT_STRING,	0,	0,	"����������"	},	// 25
	{MFT_STRING,	0,	0,	"�̹�������"	},	// 26
	{MFT_STRING,	0,	0,	"�μ��ϱ�"	},	// 27
	{MFT_SEPARATOR,	0,	0,	""		},	// 28
	{MFT_STRING,	0,	0,	"���ڼ� �����"	},	// 29
	{MFT_STRING,	0,	0,	"���ڼ� �������󰡱�"},	// 30


	//** 
// 2006.10.26 schbang
//	{MFT_STRING,	0,	0,	"��ĭ�ǰ� �Է�"},	// +25
//	{MFT_STRING,	0,	0,	"�ּҹ����� �Է�"},	// +26
//	{MFT_STRING,	0,	0,	"�ּҺ�ȭ�� �Է�"},	// +27
};

struct	_menuString g_aMenuType[] = {
	{MFT_STRING,	0,	GK_JPN,		"����Ʈ"	},
	{MFT_STRING,	0,	GK_LIN,		"����Ʈ"	},
	{MFT_STRING,	0,	GK_AVOL,	"�Ź���Ʈ"	},
	{MFT_STRING,	0,	GK_BALANCE,	"�ϸ����ǥ"	},		
	{MFT_STRING,	0,	GK_CDV,		"CandleVolume"	},
	{MFT_STRING,	0,	GK_EQV,		"EquiVolume"	},
	{MFT_STRING,	0,	GK_THREE,	"�Ｑ��ȯ��"	},
	{MFT_STRING,	0,	GK_PNF,		"P&&F��Ʈ"	},
	{MFT_STRING,	0,	GK_CLOCK,	"���ð�"	},
	{MFT_STRING,	0,	GK_PV,		"PV��Ʈ"	},
	{MFT_STRING,	0,	GK_BAR,		"����Ʈ"	},
	{MFT_STRING,	0,	GK_POLE,	"POLE��Ʈ"	},		
	{MFT_STRING,	0,	GK_KAGI,	"Kagi��Ʈ"	},
	{MFT_STRING,	0,	GK_RENKO,	"Renko��Ʈ"	},
};

struct	_menuString g_aMenuKind[] = {
	{MFT_STRING,	0,	0,	"���ã��"	},
	{MFT_STRING,	0,	16,	"�߼���ǥ"	},
	{MFT_STRING,	0,	15,	"�������ǥ"	},
	{MFT_STRING,	0,	11,	"�ŷ�����ǥ"	},
	{MFT_STRING,	0,	2,	"���尭����ǥ"	},
	{MFT_STRING,	0,	2,	"ä����ǥ"	},
	{MFT_STRING,	0,	8,	"��Ÿ��ǥ"	},
	//**
};

struct	_menuString g_aMenuAid[] = {
	{MFT_STRING,	GT_BONG,	GK_PMA,		"���� ����" },
	{MFT_STRING,	GT_BONG,	GK_NET,		"�׹���Ʈ" },
	{MFT_STRING,	GT_LINE,	GK_ADLINE,	"A/D Line" },
	{MFT_STRING,	GT_LINE,	GK_ADX,		"ADX" },
	{MFT_STRING,	GT_LINE,	GK_CCI,		"CCI" },
	{MFT_STRING,	GT_LINE,	GK_DMI,		"DMI" },
	{MFT_STRING,	GT_BONG,	GK_DEMARK,	"Demark" },
	{MFT_STRING,	GT_LINE,	GK_MACD,	"MACD" },
	{MFT_STRING,	GT_LINE,	GK_MACDOS,	"MACD Oscillator" },
	{MFT_STRING,	GT_BONG,	GK_PIVOT,	"Pivot Line" },
	{MFT_STRING,	GT_LINE,	GK_PMAO,	"Price Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_RSI,		"RSI" },
	{MFT_STRING,	GT_LINE,	GK_RSIWEIGHT, "RSI(����ġ)" },	// 20090923 : ADD : RSI Weight ��ǥ �߰�. RSI with exponentional moving average
	{MFT_STRING,	GT_LINE,	GK_TRIX,	"TRIX" },
	{MFT_STRING,	GT_LINE,	GK_WPR,		"William's %R" },	
	{MFT_STRING,	GT_BONG,	GK_PARB,	"Parabolic" },

	{MFT_STRING,	GT_LINE,	GK_GAP,		"�̰ݵ�" },
	{MFT_STRING,	GT_LINE,	GK_MOMENTUM,	"�����" },
	{MFT_STRING,	GT_LINE,	GK_ABRATIO,	"ABRatio" },
	{MFT_STRING,	GT_LINE,	GK_BANDWIDTH,	"Band Width" },
	{MFT_STRING,	GT_LINE,	GK_CO,		"Chaikin's Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_CV,		"Chaikin's Volatility" },
	{MFT_STRING,	GT_LINE,	GK_EMV,		"Ease of Movement" },
	{MFT_STRING,	GT_LINE,	GK_LFI,		"LFI" },
	{MFT_STRING,	GT_LINE,	GK_NCO,		"Net Change Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_PROC,	"Price Rate-of-Change" },
	{MFT_STRING,	GT_LINE,	GK_SIGMA,	"Sigma" },
	{MFT_STRING,	GT_LINE,	GK_SONA,	"SONAR" },
	{MFT_STRING,	GT_LINE,	GK_SONAMOMENTUM,"SONA Momentum" },
	{MFT_STRING,	GT_LINE,	GK_STOCHS,	"Slow Stochastics" },
	{MFT_STRING,	GT_LINE,	GK_STOCHF,	"Fast Stochastics" },

	{MFT_STRING,	GT_BONG,	GK_VOL,		"�ŷ���" },
	{MFT_STRING,	GT_BONG,	GK_VMA,		"�ŷ��� ����" },
	{MFT_STRING,	GT_LINE,	GK_RRATE,	"ȸ����" },
	{MFT_STRING,	GT_BONG,	GK_AMT,		"�ŷ����" },
	{MFT_STRING,	GT_BONG,	GK_AMA,		"�ŷ���� ����" },
	{MFT_STRING,	GT_LINE,	GK_ADR,		"ADR" },
	{MFT_STRING,	GT_LINE,	GK_MFI,		"Money Flow Index" },
	{MFT_STRING,	GT_LINE,	GK_OBV,		"OBV" },
	{MFT_STRING,	GT_LINE,	GK_VMAO,	"Volume Oscillator" },
	{MFT_STRING,	GT_LINE,	GK_VR,		"VR" },
	{MFT_STRING,	GT_LINE,	GK_VROC,	"Volume Rate-of-Change" },

	{MFT_STRING,	GT_LINE,	GK_PSY,		"�ɸ���" },		
	{MFT_STRING,	GT_LINE,	GK_NPSY,	"�Žɸ���" },	

	{MFT_STRING,	GT_BONG,	GK_BOLB,	"Bollinger Band" },
	{MFT_STRING,	GT_BONG,	GK_ENVL,	"Envelope" },

	{MFT_STRING,	GT_LINE,	GK_FRGNHAVE, "�ܱ��κ���(����)"},
	{MFT_STRING,	GT_LINE,	GK_FRGNRATE, "�ܱ��κ���(����)"},
	{MFT_STRING,	GT_LINE,	GK_PSNLPBUY, "���μ��ż�(����)"},
	{MFT_STRING,	GT_LINE,	GK_FRGNPBUY, "�ܱ��μ��ż�(����)"},
	{MFT_STRING,	GT_LINE,	GK_ORGNPBUY, "������ż�(����)"},
	{MFT_STRING,	GT_LINE,	GK_PSNLTBUY, "���δ������ż�(����)"},
	{MFT_STRING,	GT_LINE,	GK_FRGNTBUY, "�ܱ��δ������ż�(����)"},
	{MFT_STRING,	GT_LINE,	GK_ORGNTBUY, "����������ż�(����)"},
};

