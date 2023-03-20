#ifndef __INDICATOR_INFOMATION_H__
#define __INDICATOR_INFOMATION_H__

#define _SPIN_		'S'
#define _EDIT_		'E'
#define _NEDIT_		'N'
#define _DBEDIT_	'D'
#define _COMBO_		'C'
#define _MACOMBO_	'M'
#define _VOLCOMBO_	'V'
#define	_STATIC_	'T'
#define _FONT_		'F'
#define _COLOR_		'R'
#define _WIDTH_		'W'
#define _KIND_		'K'
#define _CHECK_		'H'

#define _SMA_		"�ܼ�����"
#define _WMA_		"��������"
#define _EMA_		"��������"

#define _OCMA_		"�ð�"
#define _HCMA_		"��"
#define _LCMA_		"����"
#define _CCMA_		"����"
#define _MCMA_		"�߰���"
#define _RCMA_		"��ǥ��"

#define _PERIOD_	"�Ⱓ"
#define _SIG_		"Signal"
#define _MATYPE_	"MA Ÿ��"
#define _OVERBUY_	"���ż�"
#define _OVERSELL_	"���ŵ�"

#define _CWK_		'A'
#define	_C_		'C'
#define _W_		'W'
#define _K_		'K'
#include "../../h/axisgenv.h"

struct	_indiInfo
{
	BYTE	kind;
	bool	hori;
	BYTE	digit;
	BYTE	bcnt;	// �⺻������ ����	

	struct	
	{
		struct	
		{
			char	kind;
			char	title[20];
			WORD	min;
			WORD	max;
		} info;
		BYTE	ccnt;		//�޺��� ���빰�� ����

		struct 
		{
			char	content[20];
			WORD	value;
		} combo[7];
		
	} basic[6];	//} basic[5];

	BYTE	lcnt;

	struct	_lineInfo
	{
		char	kind;
		char	title[20];
		WORD	value;
	} line[5];
	
	
};

static struct _indiInfo	__indiAll[] =
{
	// PMA
	{								\
		GK_PMA, false, 0,					\
		MAX_LINE, {						\
			{{_SPIN_, _PERIOD_, 0, 999}}			\
		},							\
		MAX_LINE, {						\
			{_CWK_, "MA"}					\
		}							\
	},

	// �׹���Ʈ
	{								\
		GK_NET, false, 0,					\
		4, {							\
			{{_SPIN_, "��������", 1, 999}},			\
			{{_SPIN_, "������", 1, 999}},			\
			{{_SPIN_, "���򰳼�", 1, 30}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		1, {							\
			{_CWK_, "�׹���Ʈ"}				\
		}							\
	},

	// A/D Line
	{								\
		GK_ADLINE, true, 0,					\
		2, {							\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "A/D Line"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// ADX
	{								\
		GK_ADX, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "ADX"},					\
			{_CWK_, _SIG_}					\
		}							\
	}, 

	// CCI
	{								\
		GK_CCI, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "CCI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// DMI
	{								\
		GK_DMI, true, 2,					\
		2, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "���ź��"},				\
			{_CWK_, "�϶�ź��"}				\
		}							\
	}, 
	
	// Demark
	{								\
		GK_DEMARK, false, 0,					\
		0, {'0'},						\
		2, {							\
			{_CWK_, "��ǥ��"},				\
			{_CWK_, "��ǥ����"}				\
		}							\
	},

	// MACD
	{								\
		GK_MACD, true, 2,					\
		4, {							\
			{{_SPIN_, "�ܱ�", 1, 999}},			\
			{{_SPIN_, "���", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "MACD"},				\
			{_CWK_, _SIG_}					\
		}							\
	}, 

	// MACD	Oscillator
	{								\
		GK_MACDOS, true, 2,					\
		4, {							\
			{{_SPIN_, "�ܱ�", 1, 999}},			\
			{{_SPIN_, "���", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_C_, "���"},					\
			{_C_, "�϶�"}					\
		}							\
	},

	// PMAO
	{								\
		GK_PMAO, true, 2,					\
		4, {							\
			{{_SPIN_, "�ܱ�", 1, 999}},			\
			{{_SPIN_, "���", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "PO"},					\
			{_CWK_, _SIG_}					\
		}							\
	}, 

	// RSI
	{								\
		GK_RSI, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "RSI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// RSI(����ġ) 	// 20090928 : ADD : RSI ��ǥ �߰�
	{								\
		GK_RSIWEIGHT, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "RSI(����ġ)"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// TRIX
	{								\
		GK_TRIX, true, 6,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "TRIX"},				\
			{_CWK_, _SIG_}					\
		}							\
	},
	
	// William's %R
	{								\
		GK_WPR, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "William's %R"},			\
			{_CWK_, _SIG_}					\
		}							\
	},

	// Parabolic
	{								\
		GK_PARB, false,	0,					\
		2, {							\
			{{_SPIN_, "�ִ밪", 1, 999}},			\
			{{_SPIN_, "��������", 1, 999}}			\
		},							\
		1, {							\
			{_C_, "PARABOLIC"}				\
		}							\
	},

	// Pivot
	{								\
		GK_PIVOT, false, 0,					\
		0, {'0'},						\
		5, {							\
			{_CWK_, "Pivot"},				\
			{_CWK_, "1�� ���׼�"},				\
			{_CWK_, "1�� ������"},				\
			{_CWK_, "2�� ���׼�"},				\
			{_CWK_, "2�� ������"}				\
		}							\
	},
	
	// �̰ݵ�
	{								\
		GK_GAP, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "�̰ݵ�"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// �����
	{								\
		GK_MOMENTUM, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "�����"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// ABRatio
	{								\
		GK_ABRATIO, true, 2,					\
		2, {							\
			{{_SPIN_, "�Ⱓ1", 1, 999}},			\
			{{_SPIN_, "�Ⱓ2", 1, 999}}			\
		},							\
		2, {							\
			{_CWK_, "A-Ratio"},				\
			{_CWK_, "B-Ratio"}				\
		}							\
	},

	// CO
	{								\
		GK_CO, true, 0,						\
		4, {							\
			{{_SPIN_, "�ܱ�", 1, 999}},			\
			{{_SPIN_, "���", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "CO"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// CV
	{								\
		GK_CV, true, 2,						\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "CV"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// EMV
	{								\
		GK_EMV, true, 6,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "EOM"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// LFI
	{								\
		GK_LFI, true, 2,					\
		2, {							\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "LFI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// NCO
	{								\
		GK_NCO, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "NCO"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// PROC
	{								\
		GK_PROC, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "PROC"},				\
			{_CWK_, _SIG_}					\
		}							\
	},
		
	// Sigma
	{								\
		GK_SIGMA, true,	2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "Sigma"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// SONA
	{								\
		GK_SONA, true, 2, 					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "SONAR"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// Slow Stochastics
	{								\
		GK_STOCHS, true, 2,					\
		6, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "Slow %K", 1, 999}},			\
			{{_SPIN_, "Slow %D", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "Slow %K"},				\
			{_CWK_, "Slow %D"}				\
		}							\
	},

	// Fast Stochastics
	{								\
		GK_STOCHF, true, 2,					\
		5, {							\
			{{_SPIN_, "Fast %K", 1, 999}},			\
			{{_SPIN_, "Fast %D", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "Fast %K"},				\
			{_CWK_, "Fast %D"}				\
		}							\
	},
	
	// �ŷ���
	{								\
		GK_VOL, false, 0,					\
		1, {{{_VOLCOMBO_, "����"},				\
				4, {					\
					{"���Ǿ���", 0},		\
					{"���ݴ��", GC_VOLPRC},	\
					{"�ŷ������", GC_VOLVOL},	\
					{"�ð��������", GC_VOLFPR}	\
				}					\
			}						\
		},							\
		3, {							\
			{_C_, "�ŷ���"},				\
			{_C_, "���"},					\
			{_C_, "�϶�"}					\
		}							\
	},

	// VMA
	{								\
		GK_VMA, false, 0,					\
		MAX_LINE, {						\
			{{_SPIN_, _PERIOD_, 0, 999}}			\
		},							\
		MAX_LINE, {						\
			{_CWK_, "MA"},					\
		}							
	},

	// ȸ����
	{								\
		GK_RRATE, true,	0,					\
		1, {							\
			{{_SPIN_, _SIG_, 0, 999}}			\
		},							\
		2, {							\
			{_CWK_, "ȸ����"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// �ŷ����
	{								\
		GK_AMT, false, 0,					\
		1, {{{_VOLCOMBO_, "����"},				\
				4, {					\
					{"���Ǿ���", 0},		\
					{"���ݴ��", GC_VOLPRC},	\
					{"�ŷ���ݴ��", GC_VOLVOL},	\
					{"�ð��������", GC_VOLFPR},	\
				}					\
			}						\
		},							\
		3, {							\
			{_C_, "�ŷ����"},				\
			{_C_, "���"},					\
			{_C_, "�϶�"}					\
		}							\
	},

	// AMA
	{								\
		GK_AMA, false, 0,					\
		MAX_LINE, {						\
			{{_SPIN_, _PERIOD_, 0, 999}}			\
		},							\
		MAX_LINE, {						\
			{_CWK_, "MA"},					\
		}							
	},

	// ADR
	{								\
		GK_ADR, true, 0,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "ADR"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// MFI
	{								\
		GK_MFI, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "MFI"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// OBV
	{								\
		GK_OBV, true, 0,					\
		2, {							\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "OBV"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// VR
	{								\
		GK_VR, true, 2,						\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "VR"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// VROC
	{								\
		GK_VROC, true, 2,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "VROC"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// VMAO
	{								\
		GK_VMAO, true, 2,					\
		4, {							\
			{{_SPIN_, "�ܱ�", 1, 999}},			\
			{{_SPIN_, "���", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "VO"},					\
			{_CWK_, _SIG_}					\
		}							\
	},

	// �ɸ���
	{								\
		GK_PSY, true, 0,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "�ɸ���"},				\
			{_CWK_, _SIG_}					\
		}							\
	},

	// �Žɸ���
	{								\
		GK_NPSY, true, 2,					\
		5, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			},						\
			{_COLOR_, _OVERBUY_},				\
			{_COLOR_, _OVERSELL_}				\
		},							\
		2, {							\
			{_CWK_, "�Žɸ���"},				\
			{_CWK_, _SIG_}					\
		}							\
	},	

	// Bollinger Band
	{								\
		GK_BOLB, false,	0,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "�¼�", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		3, {							\
			{_CWK_, "���Ѽ�"},				\
			{_CWK_, "�߽ɼ�"},				\
			{_CWK_, "���Ѽ�"}				\
		}							\
	},
	
	// Envelope
	{								\
		GK_ENVL, false,	0,					\
		3, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "�¼�", 1, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		3, {							\
			{_CWK_, "���Ѽ�"},				\
			{_CWK_, "�߽ɼ�"},				\
			{_CWK_, "���Ѽ�"}				\
		}							\
	},


	// ĵ�麼��
	{								\
		GK_CDVV, true, 0,					\
		0, {'0'},						\
		1, {							\
			{_CWK_, "ĵ�麼��"}				\
		}							\
	},
	// ��������
	{								\
		GK_EQVV, true, 0,					\
		0, {'0'},						\
		1, {							\
			{_CWK_, "��������"},				\
		}							\
	},
	// Band Width
	{								\
		GK_BANDWIDTH, true,	2,				\
		4, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "�¼�", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "Band Width"},				\
			{_CWK_, _SIG_}					\
		}							\
	},
	// SONA Momentum
	{								\
		GK_SONAMOMENTUM, true, 2, 				\
		4, {							\
			{{_SPIN_, _PERIOD_, 1, 999}},			\
			{{_SPIN_, "�Ⱓ2", 1, 999}},			\
			{{_SPIN_, _SIG_, 0, 999}},			\
			{{_MACOMBO_, _MATYPE_},				\
				3, {					\
					{_SMA_, 0},			\
					{_WMA_, GC_MAWEIGHT},		\
					{_EMA_, GC_MAEXP}		\
				}					\
			}						\
		},							\
		2, {							\
			{_CWK_, "SONA Momentum"},			\
			{_CWK_, _SIG_}					\
		}							\
	},

	// �ܱ��κ�����Ȳ
	{
		GK_FRGNHAVE, true, 0,
		0, {'0'},
		1, {
			{_CWK_, "�ܱ���(������)" }
		}
	},
	// �ܱ��κ�������
	{
		GK_FRGNRATE, true, 0,
		0, {'0'},
		1, {
			{_CWK_, "�ܱ���(����)"}
		}
	},
	// ���μ��Ÿż���
	{
		GK_PSNLPBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "���� ���ż�(����)"},
			{_CWK_, "���� ���ŵ�(����)"}
		}
	},
	// �ܱ��μ��Ÿż���
	{
		GK_FRGNPBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "�ܱ��� ���ż�(����)"},
			{_CWK_, "�ܱ��� ���ŵ�(����)"}
		}
	},
	// ������Ÿż���
	{
		GK_ORGNPBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "��� ���ż�(����)"},
			{_CWK_, "��� ���ŵ�(����)"}
		}
	},
	// ���δ������Ÿż���
	{
		GK_PSNLTBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "���� ���ż�(����)"},
			{_CWK_, "���� ���ŵ�(����)"}
		}
	},
	// �ܱ��δ������Ÿż���
	{
		GK_FRGNTBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "�ܱ��� ���ż�(����)"},
			{_CWK_, "�ܱ��� ���ŵ�(����)"}
		}
	},
	// ����������Ÿż���
	{
		GK_ORGNTBUY, true, 0,
		0, {'0'},
		2, {
			{_CWK_, "��� ���ż�(����)"},
			{_CWK_, "��� ���ŵ�(����)"}
		}
	},
};

#endif