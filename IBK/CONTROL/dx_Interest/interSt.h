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
//  Components   : interStruct.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2002-10	Initial version
// *****************************************************************************
//
// 
//
#if !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)
#define INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//	user/userName����
//	file name : portfolio.i00 ~ portfolio.i99
//

struct _inters
{
	char	gubn;		// ���񱸺�
				// 0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	name[30];	// �����
	int	possNum;	// ��������
	double	possPrice;	// �����ܰ�
	char	memo[30];	// �޸�
	char	o_check;	// �ֹ� ���࿩��
				// 0:none, 1:checked
	char	o_kind;		// �ֹ�����
				// 1:�ŵ�, 2:�ż�
	char	o_mmgb;		// �Ÿű���
				// 1:����, 2:���尡
	int	o_num;		// �ֹ�����
	double	o_price;	// �ֹ��ܰ�
	char	a_kind;		// �˶� ����
				// 1:�׸���, 2:�׸��� �Ҹ�
	int	a_num;		// ���� �׸� ����
	char	filler[100];	// Reserved
};

struct _alarms
{
	char	a_kind;		// �����׸�
				// 0:none, 1:���簡, 2:�ŵ�ȣ��, 3:�ż�ȣ��, 4:�����, 5:������, 6:���ͷ�
	double	a_value;	// ���� �񱳰�
	char	a_condition;	// ���� ����
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40];	// Reserved
};

#endif // !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)