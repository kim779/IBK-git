
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
//  Components   : key.hxx
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2004-03	Initial version
// *****************************************************************************

#ifndef	_KEY_HXX_
#define	_KEY_HXX_

#include "symbol.h"
#include "index.hxx"

struct	_keys {
	int	key;
	int	index;
};

struct	_keys	keys[] = {
	{ DHG1, askPrice1		},	// �ŵ�ȣ��
	{ DHG2, askPrice2		},
	{ DHG3, askPrice3		},
	{ DHG4, askPrice4		},
	{ DHG5, askPrice5		},
	{ DHG6, askPrice6		},
	{ DHG7, askPrice7		},
	{ DHG8, askPrice8		},
	{ DHG9, askPrice9		},
	{ DHGA, askPrice10		},

	{ DHJ1, askSize1		},	// �ŵ�ȣ�� �ܷ�
	{ DHJ2, askSize2		},
	{ DHJ3, askSize3		},
	{ DHJ4, askSize4		},
	{ DHJ5, askSize5		},
	{ DHJ6, askSize6		},
	{ DHJ7, askSize7		},
	{ DHJ8, askSize8		},
	{ DHJ9, askSize9		},
	{ DHJA, askSize10		},

	{ DHX1, askBefore1		},	// �ŵ�ȣ�� �ܷ����
	{ DHX2, askBefore2		},
	{ DHX3, askBefore3		},
	{ DHX4, askBefore4		},
	{ DHX5, askBefore5		},
	{ DHX6, askBefore6		},
	{ DHX7, askBefore7		},
	{ DHX8, askBefore8		},
	{ DHX9, askBefore9		},
	{ DHXA, askBefore10		},

	{ SHG1, bidPrice1		},	// �ż�ȣ��
	{ SHG2, bidPrice2		},
	{ SHG3, bidPrice3		},
	{ SHG4, bidPrice4		},
	{ SHG5, bidPrice5		},
	{ SHG6, bidPrice6		},
	{ SHG7, bidPrice7		},
	{ SHG8, bidPrice8		},
	{ SHG9, bidPrice9		},
	{ SHGA, bidPrice10		},

	{ SHJ1, bidSize1		},	// �ż�ȣ�� �ܷ�
	{ SHJ2, bidSize2		},
	{ SHJ3, bidSize3		},
	{ SHJ4, bidSize4		},
	{ SHJ5, bidSize5		},
	{ SHJ6, bidSize6		},
	{ SHJ7, bidSize7		},
	{ SHJ8, bidSize8		},
	{ SHJ9, bidSize9		},
	{ SHJA, bidSize10		},

	{ SHX1, bidBefore1		},	// �ż�ȣ�� �ܷ����
	{ SHX2, bidBefore2		},
	{ SHX3, bidBefore3		},
	{ SHX4, bidBefore4		},
	{ SHX5, bidBefore5		},
	{ SHX6, bidBefore6		},
	{ SHX7, bidBefore7		},
	{ SHX8, bidBefore8		},
	{ SHX9, bidBefore9		},
	{ SHXA, bidBefore10		},
	
	{ HTIM, cTime			},	// ȣ���ð�

	{ DHCJ, askTotalSize		},	// ���ܷ�
	{ DHXX, askTotalBefore		},
	{ SHCJ, bidTotalSize		},
	{ SHXX, bidTotalBefore		},

	{ SIGA, openPrice		},	// �ð�
	{ KOGA, highPrice		},	// ��
	{ JEGA, lowPrice		},	// ����
	{ DIFF, preBefore		},	// ���ϴ��
	{ UDYL, prePercent		},	// �����

	{ YSCA, prePrice		},	// ����
	{ YSCV, preVolume		},

	{ DHTJ, askSizeTime		},	// �ð���
	{ DHTX, askSizeTimeBefore	},
	{ SHTJ, bidSizeTime		},
	{ SHTX, bidSizeTimeBefore	},
	{ 515,  gajungPrice		},	// ���� ��հ�
	{ DSCHA, sizeDiff		},
	{ GVOL, gVolume			},
	{ GAMT, gAMT			},	// �ŷ����

	{ DLP1, askLp1			},	// �ŵ� LP �ܷ�
	{ DLP2, askLp2			},
	{ DLP3, askLp3			},
	{ DLP4, askLp4			},
	{ DLP5, askLp5			},
	{ DLP6, askLp6			},
	{ DLP7, askLp7			},
	{ DLP8, askLp8			},
	{ DLP9, askLp9			},
	{ DLPA, askLp10			},

	{ SLP1, bidLp1			},	// �ż� LP �ܷ�
	{ SLP2, bidLp2			},
	{ SLP3, bidLp3			},
	{ SLP4, bidLp4			},
	{ SLP5, bidLp5			},
	{ SLP6, bidLp6			},
	{ SLP7, bidLp7			},
	{ SLP8, bidLp8			},
	{ SLP9, bidLp9			},
	{ SLPA, bidLp10			},

	{ DLPX, askTotalLp		},	// LP ���ܷ�
	{ SLPX, bidTotalLp		},
	{ CPOW, chegang			},  // ü�ᰭ��
	{ SVIU, SVIUP			},  // ����vi �����°�
	{ SVID, SVIDN			},  // ����vi �����϶���
};

struct	_keys	keyx[] = {
	{ EDHG1, askPrice1		},	// �ŵ�ȣ��
	{ EDHG2, askPrice2		},
	{ EDHG3, askPrice3		},
	{ EDHG4, askPrice4		},
	{ EDHG5, askPrice5		},
	{ EDHG6, askPrice6		},
	{ EDHG7, askPrice7		},
	{ EDHG8, askPrice8		},
	{ EDHG9, askPrice9		},
	{ EDHGA, askPrice10		},

	{ EDHJ1, askSize1		},	// �ŵ�ȣ�� �ܷ�
	{ EDHJ2, askSize2		},
	{ EDHJ3, askSize3		},
	{ EDHJ4, askSize4		},
	{ EDHJ5, askSize5		},
	{ EDHJ6, askSize6		},
	{ EDHJ7, askSize7		},
	{ EDHJ8, askSize8		},
	{ EDHJ9, askSize9		},
	{ EDHJA, askSize10		},

	{ EDHX1, askBefore1		},	// �ŵ�ȣ�� �ܷ����
	{ EDHX2, askBefore2		},
	{ EDHX3, askBefore3		},
	{ EDHX4, askBefore4		},
	{ EDHX5, askBefore5		},
	{ EDHX6, askBefore6		},
	{ EDHX7, askBefore7		},
	{ EDHX8, askBefore8		},
	{ EDHX9, askBefore9		},
	{ EDHXA, askBefore10		},

	{ ESHG1, bidPrice1		},	// �ż�ȣ��
	{ ESHG2, bidPrice2		},
	{ ESHG3, bidPrice3		},
	{ ESHG4, bidPrice4		},
	{ ESHG5, bidPrice5		},
	{ ESHG6, bidPrice6		},
	{ ESHG7, bidPrice7		},
	{ ESHG8, bidPrice8		},
	{ ESHG9, bidPrice9		},
	{ ESHGA, bidPrice10		},

	{ ESHJ1, bidSize1		},	// �ż�ȣ�� �ܷ�
	{ ESHJ2, bidSize2		},
	{ ESHJ3, bidSize3		},
	{ ESHJ4, bidSize4		},
	{ ESHJ5, bidSize5		},
	{ ESHJ6, bidSize6		},
	{ ESHJ7, bidSize7		},
	{ ESHJ8, bidSize8		},
	{ ESHJ9, bidSize9		},
	{ ESHJA, bidSize10		},

	{ ESHX1, bidBefore1		},	// �ż�ȣ�� �ܷ����
	{ ESHX2, bidBefore2		},
	{ ESHX3, bidBefore3		},
	{ ESHX4, bidBefore4		},
	{ ESHX5, bidBefore5		},
	{ ESHX6, bidBefore6		},
	{ ESHX7, bidBefore7		},
	{ ESHX8, bidBefore8		},
	{ ESHX9, bidBefore9		},
	{ ESHXA, bidBefore10		},
	
	{ EHTIM, cTime			},	// ȣ���ð�

	{ EDHCJ, askTotalSize		},	// ���ܷ�
	{ EDHXX, askTotalBefore		},
	{ ESHCJ, bidTotalSize		},
	{ ESHXX, bidTotalBefore		},

	{ ESIGA, openPrice		},	// �ð�
	{ EKOGA, highPrice		},	// ��
	{ EJEGA, lowPrice		},	// ����
	{ EDIFF, preBefore		},	// ���ϴ��
	{ EUDYL, prePercent		},	// �����

	{ EYSCA, prePrice		},	// ����
	{ EYSCV, preVolume		},

	{ EDHTJ, askSizeTime		},	// �ð���
	{ EDHTX, askSizeTimeBefore	},
	{ ESHTJ, bidSizeTime		},
	{ ESHTX, bidSizeTimeBefore	},
	{ DSCHA, sizeDiff		},
	{ EGVOL, gVolume		},
	{ EGAMT, gAMT			}	// �ŷ����
};


#endif


