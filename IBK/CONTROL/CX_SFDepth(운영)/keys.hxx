
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

	{ DHC1, askBefore1		},	// �ŵ�ȣ�� �Ǽ�
	{ DHC2, askBefore2		},
	{ DHC3, askBefore3		},
	{ DHC4, askBefore4		},
	{ DHC5, askBefore5		},
	{ DHC6, askBefore6		},
	{ DHC7, askBefore7		},
	{ DHC8, askBefore8		},
	{ DHC9, askBefore9		},
	{ DHCA, askBefore10		},

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

	{ SHC1, bidBefore1		},	// �ż�ȣ�� �Ǽ�
	{ SHC2, bidBefore2		},
	{ SHC3, bidBefore3		},
	{ SHC4, bidBefore4		},
	{ SHC5, bidBefore5		},
	{ SHC6, bidBefore6		},
	{ SHC7, bidBefore7		},
	{ SHC8, bidBefore8		},
	{ SHC9, bidBefore9		},
	{ SHCA, bidBefore10		},
	
	{ HTIM, cTime			},	// ȣ���ð�

	{ DHCJ, askTotalSize		},	// ���ܷ�
	{ DHCC, askTotalBefore		},	// �ѰǼ�			
	{ SHCJ, bidTotalSize		},
	{ SHCC, bidTotalBefore		},
	{ DSCHA, sizeDiff		},

	{ SIGA, openPrice		},	// �ð�
	{ KOGA, highPrice		},	// ��
	{ JEGA, lowPrice		},	// ����

	{ YSCA, prePrice		},  //����

	{ DHTJ, askSizeTime		},	// �ð���
	{ DHTX, askSizeTimeBefore	},
	{ SHTJ, bidSizeTime		},
	{ SHTX, bidSizeTimeBefore	},
	{ 515,  gajungPrice		},	// ���� ��հ�
	{ MGJY, infoMGJV		},	// �̰�
	{ MDIF, infoMGDF		},	// �̰� ���
	{ SBAS, infoBASE		},	// ���̽ý�
	{ GRAT,	infoRATE		},	// ������
	{ GIRG, infoIRGA		},	// �̷а�
	{ NJBD,	infoNJBD		},	// ���纯����
	{ DLTA, infoDELT		},	// ��Ÿ
	{ GAMA, infoGAMM		},	// ����
	{ RSHG, infoRSH			},  // �ǽð����Ѱ�
	{ RHHG, infoRHH			},  // �ǽð����Ѱ�

	{ CURR, infoCURR		},	// ���簡
	{ DIFF,	infoDIFF		},	// ���
	{ UDYL, infoUDYL		},	// �����
	{ GVOL,	infoGVOL		},	// �ŷ���
};
#endif